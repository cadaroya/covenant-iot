#include "WiFiEsp.h"
#include "SoftwareSerial.h"

#define vardisp(x,y) \
  if (debug_cov) {\
  Serial.print(F(x));\
  Serial.print(F("> "));\
  Serial.println(y);}
#define vardisp2(x,y,z) \
  if (debug_cov) {\
  Serial.print(F(x));\
  Serial.print(F("> "));\
  Serial.print(F(y));\
  Serial.println(z);}
#define var(x) Serial.println(x)
#define connLed 8
#define sensPin A0
#define fanPin A5
#define fanOff vardisp("I","Turning off fan");digitalWrite(fanPin,LOW);runningFan=false
#define fanOn vardisp("I","Turning on fan");digitalWrite(fanPin,HIGH);runningFan=true
#define sensorRead sensorVal=analogRead(sensPin);vardisp("R",sensorVal);
SoftwareSerial Serial1(2,3);


/* PROGRAM SETTINGS */
const char ssid[]                   = "cs131";
const char pass[]                   = "urd1(31)4me<3";
const char host[]                   = "immense-plateau-44759.herokuapp.com";
#define port                        80
#define postingInterval             10000L  //1000L = 1 second
#define readInterval                1000L
#define reconnectTimeout            30000L
#define maxTries                    1
#define threshold                   40
#define usePing                     false
#define debug_cov                   true


/* MISC. GLOBAL VARIABLES */
WiFiEspClient client;
int wifiStatus = WL_IDLE_STATUS;
String macStr;
unsigned long lastConnectionTime = 0;  
unsigned long lastReadTime = 0;
unsigned long lastTransTime = 0;
unsigned long lastResetTime = 0;
unsigned long lastWifiTime = 0;
int sensorVal = 0;
bool sensorOnly = false;
bool runningFan = false;
bool lastState = false;
const int leds[] = {4,5,6,7};
// 2, 4, 6-minute intervals for LED indicators
//const unsigned long ledTimes[] = {120000,240000,360000};
const unsigned long ledTimes[] = {5000,10000,15000};


/* FUNCTIONS */

void setLed(int i) {
  for (int j=0; j<=3; j++) {
    if (i == j) {
      digitalWrite(leds[j], HIGH);
    }
    else {
      digitalWrite(leds[j], LOW);
    }
  }
}

bool tryWifiConn() {
  int tries = 0;
  do {
    WiFi.disconnect();
    vardisp("Try", tries);
    if (tries < maxTries) {
      vardisp2("I", "Connecting to SSID: ", ssid);
      wifiStatus = WiFi.begin(ssid, pass);
      tries += 1;
    }
    else {
      vardisp2("E", "Can't connect to SSID: ", ssid);
      return false;
    }
  } while (wifiStatus != WL_CONNECTED);
  vardisp2("I", "Connected to SSID: ", ssid);
  return true;
}

void setup()
{
  pinMode(sensPin, INPUT);
  pinMode(fanPin, OUTPUT);
  for (int i=0; i<=3; i++) {
    pinMode(leds[i], OUTPUT);
  }
  pinMode(connLed, OUTPUT);
  
  Serial.begin(9600);
  while (!Serial);
  vardisp("\nI", "Serial port ready");
  
  Serial1.begin(9600);  // ESP-12E only supports up to 9600 baud
  while (!Serial1);
  vardisp("I", "WiFi serial port ready");
  
  vardisp("I", "Initializing WiFi serial port");
  WiFi.init(&Serial1);
  vardisp("I", "WiFi serial port initialized");

  if (WiFi.status() == WL_NO_SHIELD) {
    vardisp("E", "WiFi shield not present");
    vardisp("W", "Board will now run in sensor-only mode");
    sensorOnly = true;
    lastWifiTime = millis();
  }

  if (!sensorOnly) {
    if (tryWifiConn()) {
      digitalWrite(connLed, HIGH);
      printWifiStatus();
    }
    else {
      digitalWrite(connLed, LOW);
      vardisp("W", "Board will now run in sensor-only mode");
      sensorOnly = true;
      lastWifiTime = millis();
    }
  }

  sensorRead;
  if (sensorVal >= threshold) {
    lastState = true;
    lastTransTime = millis();
  }
  lastReadTime = millis() - readInterval - 100;
}


void loop()
{
  if (millis() - lastReadTime > readInterval) {
    sensorRead;
    unsigned long t = millis() - lastTransTime;
    if (sensorVal >= threshold) {
      if (!lastState) {
        vardisp("I", "Sensor level threshold reached, starting countdown");
        lastState = true;
        lastTransTime = millis();
        t = 0;
      }
      if (t <= ledTimes[0]){
        setLed(0);
      }
      else if (t > ledTimes[0] && t <= ledTimes[1]){
        setLed(1);
        if (!runningFan) {
          fanOn;
        }
      }
      else if (t > ledTimes[1] && t <= ledTimes[2]){
        setLed(2);
      }
      else if (t > ledTimes[2]){
        setLed(3);
      }
    }
    else {
      if (lastState) {
        vardisp("I", "Sensor level below threshold, stopping countdown");
        lastState = false;
        lastResetTime = millis();
      }
      unsigned long t2 = millis() - lastResetTime;
      if (t2 > ledTimes[0]) {
        setLed(0);
        if (runningFan) {
          fanOff;
        }
      }
    }
    lastReadTime = millis();
  }

  if (!sensorOnly) {
    if (debug_cov) {
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
    }
    if (millis() - lastConnectionTime > postingInterval) {
      if (usePing) {
        if (WiFi.ping(host)) {
          httpRequest();
        }
        else {
          vardisp2("W", "Server ping failed ", millis()/1000);
        }
      }
      else {
        httpRequest();
      }
    }
  }
  else {
    if (millis() - lastWifiTime > reconnectTimeout) {
      if (tryWifiConn()) {
        digitalWrite(connLed, HIGH);
        vardisp("W", "Board will now run in WiFi polling mode");
        printWifiStatus();
        sensorOnly = false;
      }
      else {
        digitalWrite(connLed, LOW);
      }
    }
  }
}


void httpRequest()
{
  client.stop();
  vardisp("I", "Connecting to server:");
  vardisp2("I", "Hostname: ", host);
  vardisp2("I", "Port: ", port);
  if (client.connect(host, port)) {
    vardisp("I", "Connected, sending data to server");
    digitalWrite(connLed, HIGH);

    char* postdata1 = (char*) malloc(sizeof(char) * 50);
    sprintf(postdata1, "{\"");
    if (debug_cov) {
      sprintf(postdata1, "%s%s\":%s", postdata1, "hardware_id", "12345");
      sprintf(postdata1, "%s,\"%s\":%s", postdata1, "token", "7397");
    }
    else {
      sprintf(postdata1, "%s%s\":%s", postdata1, "hardware_id", macStr);
      sprintf(postdata1, "%s,\"%s\":%d", postdata1, "token", "145");
    }
    char* postdata2 = (char*) malloc(sizeof(char) * 50);
    sprintf(postdata2, ",\"%s\":%d", "analog_reading", sensorVal);
    sprintf(postdata2, "%s,\"%s\":%d", postdata2, "threshold", threshold);
    sprintf(postdata2, "%s}", postdata2);
    vardisp("M", postdata1);
    vardisp("M", postdata2);
    int totLen = strlen(postdata1) + strlen(postdata2);

    client.print("POST /api/v1/reading HTTP/1.1\r\n"
      "Host: ");
    client.print(host);
    client.print("\r\nUser-Agent: ACEduino/2.1\r\n"
      "Content-Type: application/json\r\n"
      "Content-Length: ");
    client.print(totLen);
    client.print("\r\nConnection: close\r\n\r\n");
    client.print(postdata1);
    client.print(postdata2);

    //free(postdata);
    free(postdata1);
    free(postdata2);

    vardisp("I", "Data sent to server");
    lastConnectionTime = millis();
  }
  else {
    vardisp("W", "Connection failed...");
    digitalWrite(connLed, LOW);
    vardisp("I", "Resetting WiFi module");
    WiFi.reset();
    delay(1000);
    if (tryWifiConn()) {
      digitalWrite(connLed, HIGH);
      printWifiStatus();
    }
    else {
      digitalWrite(connLed, LOW);
      vardisp("W", "Board will now run in sensor-only mode");
      sensorOnly = true;
    }
  }
}


void printWifiStatus()
{
  byte mac[6];
  WiFi.macAddress(mac);
  macStr = String();
  for (int i=5; i>=0; i--) {
    if (int(mac[i])<16) {
      macStr.concat(String(0));
    }
    macStr.concat(String(int(mac[i]), HEX));
  }
  vardisp2("I", "MAC: ", macStr);
  vardisp2("I", "SSID: ", WiFi.SSID());
  vardisp2("I", "IP: ", WiFi.localIP());
  vardisp2("I", "RSSI: ", WiFi.RSSI());
}

