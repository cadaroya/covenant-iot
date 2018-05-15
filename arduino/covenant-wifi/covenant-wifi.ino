/*
 *  Project COvenant
 *  2nd Sem CS 145 Capstone 2017-2018
 *  Clemente, Cruz, Daroya, Mariano, Segismundo
 */


 /*
  * LIBRARIES
  */

#include "WiFiEsp.h"
#include "SoftwareSerial.h"


/*
 * DEFINITIONS
 */

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
#define var(x) if(debug_cov){ Serial.print(x); }
#define connLed 8
#define sensPin A0
#define fanPin A5
#define fanOff vardisp("I","Turning off fan");digitalWrite(fanPin,LOW);runningFan=false
#define fanOn vardisp("I","Turning on fan");digitalWrite(fanPin,HIGH);runningFan=true
#define sensorRead sensorVal=getReading();vardisp("R",sensorVal);

#define SerialBaud 9600
#define EspBaud 9600

#define wifiName ESP8266.println(F("AT+CWHOSTNAME=COvenant_145"));
#define wifiSleep ESP8266.println(F("AT+SLEEP=2"));
#define wifiWake ESP8266.println(F("AT+SLEEP=0"));
#define wifiMaxpow ESP8266.println(F("AT+RFPOWER=82"));
#define wifiReset WiFi.reset();wifiMaxpow;wifiName;


/*
 * PROGRAM SETTINGS
 */
 
/*const char ssid[]                   = "cs131";
const char pass[]                   = "urd1(31)4me<3";*/
const char ssid[]                   = "retardis";
const char pass[]                   = "sigepala";
const char host[]                   = "immense-plateau-44759.herokuapp.com";
#define port                        80
#define readInterval                1000L
#define reconnectTimeout            10000L
#define maxTries                    1
#define integerValue                false
#define R0                          0.07
//#define threshold                   25
#define threshold                   8
#define usePing                     true
#define pingServer                  "1.1.1.1"
#define debug_cov                   true


/*
 * MISC. GLOBALS
 */
 
SoftwareSerial ESP8266(2,3);
WiFiEspClient client;
int wifiStatus = WL_IDLE_STATUS;
unsigned long lastReadTime = 0;
unsigned long lastTransTime = 0;
unsigned long lastResetTime = 0;
unsigned long lastWifiTime = 0;
double sensorVal = 0;
bool sensorOnly = false;
bool runningFan = false;
int state = 0;
const int leds[] = {4,5,6,7};
//const unsigned long ledTimes[] = {120000,240000,360000};
const unsigned long ledTimes[] = {10000,20000,30000};


/*
 * FUNCTIONS
 */

double getReading() {
  double val = analogRead(sensPin);
  double sensVolt = (val/1024.0)*5.0;
  double rs = (5.0-sensVolt)/sensVolt;
  double ratio = rs/R0;
  double ppm = 50.0 * pow((1.667 / ratio), 1.0/0.6661);
  if (integerValue) {
    return round(ppm);
  }
  return ppm;
}

void setLed(int i) {
  if (i < 0) {
    for (int j=0; j<=3; j++) {
      digitalWrite(leds[j], LOW);
    }
  }
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
  wifiWake;
  int tries = 0;
  do {
    WiFi.disconnect();
    if (tries < maxTries) {
      vardisp2("I", "Connecting to SSID: ", ssid);
      wifiStatus = WiFi.begin(ssid, pass);
      tries += 1;
    }
    else {
      wifiReset;
      vardisp("E", "Can't connect to SSID");
      digitalWrite(connLed, LOW);
      wifiSleep;
      return false;
    }
    vardisp("T", tries);
  } while (wifiStatus != WL_CONNECTED);
  vardisp("I", "Connected to SSID");
  if (usePing) {
    vardisp("I", "Checking Internet connection");
    if (WiFi.ping(pingServer)) {
      digitalWrite(connLed, HIGH);
      vardisp("I", "Connected to the Internet");
    } else {
      digitalWrite(connLed, LOW);
      vardisp("W", "Not connected to the Internet");
      wifiSleep;
      return false;
    }
  }
  return true;
}

void setup() {
  pinMode(sensPin, INPUT);
  pinMode(fanPin, OUTPUT);
  for (int i=0; i<=3; i++) {
    pinMode(leds[i], OUTPUT);
  }
  pinMode(connLed, OUTPUT);
  
  Serial.begin(SerialBaud);
  while (!Serial);
  vardisp("\nI", "Serial port ready");

  ESP8266.begin(EspBaud);  // ESP-12E only supports up to 9600 baud
  while (!ESP8266);
  vardisp("I", "WiFi serial port ready");

  delay(1000);
  
  vardisp("I", "Initializing WiFi serial port");
  WiFi.init(&ESP8266);
  vardisp("I", "WiFi serial port initialized");

  if (WiFi.status() == WL_NO_SHIELD) {
    vardisp("E", "WiFi shield not present");
    vardisp("W", "Board will now run in sensor-only mode");
    sensorOnly = true;
    lastWifiTime = millis();
  }

  if (!sensorOnly) {
    wifiMaxpow;
    //ESP8266.println(F("AT+CWMODE=1"));
    //ESP8266.println(F("AT+CWSTARTSMART"));
    
    if (tryWifiConn()) {
      printWifiStatus();
    }
    else {
      vardisp("W", "Board will now run in sensor-only mode");
      sensorOnly = true;
      lastWifiTime = millis();
    }
  }

  sensorRead;
  if (sensorVal >= threshold) {
    state = 1;
    lastTransTime = millis();
  }
  lastReadTime = millis() - readInterval - 100;

  if (!sensorOnly) {
    httpRequest();
  }
}

void loop() {
  bool isTransition = false;
  if (millis() - lastReadTime > readInterval) {
    sensorRead;
    unsigned long t = millis() - lastTransTime;
    if (sensorVal >= threshold) {
      if (state == 0) {
        vardisp("I", "Sensor level threshold reached, starting countdown");
        setLed(0);
        isTransition = true;
        state = 1;
        lastTransTime = millis();
        t = 0;
        vardisp("S", state);
      }
      if (t <= ledTimes[0]){
        setLed(0);
        if (state != 1) {
          isTransition = true;
          vardisp("S", state);
          state = 1;
        }
      }
      else if (t > ledTimes[0] && t <= ledTimes[1]){
        setLed(1);
        if (state != 2) {
          isTransition = true;
          vardisp("S", state);
          state = 2;
        }
        if (!runningFan) {
          fanOn;
        }
      }
      else if (t > ledTimes[1] && t <= ledTimes[2]){
        setLed(2);
        if (state != 3) {
          isTransition = true;
          vardisp("S", state);
          state = 3;
        }
      }
      else if (t > ledTimes[2]){
        setLed(3);
        if (state != 4) {
          isTransition = true;
          vardisp("S", state);
          state = 4;
        }
      }
    }
    else {
      if (state != 0) {
        vardisp("I", "Sensor level below threshold, stopping countdown");
        isTransition = true;
        state = 0;
        vardisp("S", state);
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
    if (isTransition) {
      httpRequest();
    }
  }
  else {
    if (millis() - lastWifiTime > reconnectTimeout) {
      if (usePing) {
        if (!WiFi.ping(pingServer)) {
          if (tryWifiConn()) {
            vardisp("I", "Board will now run in WiFi polling mode");
            printWifiStatus();
            digitalWrite(connLed, HIGH);
            sensorOnly = false;
            isTransition = true;
          }
        }
      }
      else {
        if (tryWifiConn()) {
          vardisp("I", "Board will now run in WiFi polling mode");
          printWifiStatus();
          digitalWrite(connLed, HIGH);
          sensorOnly = false;
          isTransition = true;
        }
      }
      if (isTransition) {
        httpRequest();
      }
    }
  }
}

void httpRequest()
{
  wifiWake;
  client.stop();
  vardisp("I", "Connecting to server:");
  vardisp2("I", "Hostname: ", host);
  vardisp2("I", "Port: ", port);
  if (client.connect(host, port)) {
    if (usePing) {
      if (!WiFi.ping(pingServer)) {
        digitalWrite(connLed, LOW);
        vardisp("W", "Internet connection lost");
        vardisp("W", "Board will now run in sensor-only mode");
        lastWifiTime = millis();
        digitalWrite(connLed, LOW);
        sensorOnly = true;
      }
    }
    vardisp("I", "Connected, sending data to server");
    //var(F("=======================================================================\n"));
    digitalWrite(connLed, HIGH);

    client.print("POST /api/v1/reading HTTP/1.1\r\nHost: ");
    //var(F("POST /api/v1/reading HTTP/1.1\r\nHost: "));
    client.print(host);
    //var(host);
    client.print("\r\nUser-Agent: ACEduino/2.1\r\n");
    //var(F("\r\nUser-Agent: ACEduino/2.1\r\n"));
    client.print("Content-Type: application/json\r\n"
      "Content-Length: ");
    //var(F("Content-Type: application/json\r\n"
    //    "Content-Length: "));

    char* postdata1 = (char*) malloc(sizeof(char) * 50);
    sprintf_P(postdata1, PSTR("{\""));
    if (debug_cov) {
      sprintf_P(postdata1, PSTR("%s%s\":%s"), postdata1, "hardware_id", "12345");
      sprintf_P(postdata1, PSTR("%s,\"%s\":%s"), postdata1, "token", "7397");
    }
    else {
      sprintf_P(postdata1, PSTR("%s%s\":%s"), postdata1, "hardware_id", "145");
      sprintf_P(postdata1, PSTR("%s,\"%s\":%d"), postdata1, "token", "145");
    }
    char* postdata2 = (char*) malloc(sizeof(char) * 50);
    if (integerValue) {
      sprintf_P(postdata2, PSTR(",\"%s\":%d"), "analog_reading", (int)sensorVal);
    }
    else {
      char str_temp[8];
      dtostrf(sensorVal, 4, 2, str_temp);
      sprintf_P(postdata2, PSTR(",\"%s\":%s"), "analog_reading", str_temp);
    }
    if (state > 0) {
      sprintf_P(postdata2, PSTR("%s,\"%s\":%d"), postdata2, "threshold", state-1);
    }
    else {
      sprintf_P(postdata2, PSTR("%s,\"%s\":%d"), postdata2, "threshold", state);
    }
    sprintf_P(postdata2, PSTR("%s}"), postdata2);
    
    client.print(strlen(postdata1) + strlen(postdata2));
    //var(strlen(postdata1) + strlen(postdata2));
    client.print("\r\nConnection: close\r\n\r\n");
    //var(F("\r\nConnection: close\r\n\r\n"));
    client.print(postdata1);
    //var(postdata1);
    client.print(postdata2);
    //var(postdata2);

    free(postdata1);
    free(postdata2);
    
    //var(F("\n=======================================================================\n"));
    vardisp("I", "Data sent to server");
    
    /*if (debug_cov) {
      var(F("=======================================================================\n"));
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
      var(F("\n=======================================================================\n"));
    }*/
  }
  else {
    vardisp("W", "Connection to server failed");
    digitalWrite(connLed, LOW);
    vardisp("I", "Resetting WiFi module");
    wifiReset;
    delay(1000);
    wifiWake;
    if (tryWifiConn()) {
      printWifiStatus();
    }
    else {
      digitalWrite(connLed, LOW);
      vardisp("W", "Board will now run in sensor-only mode");
      sensorOnly = true;
    }
  }
  wifiSleep;
}


void printWifiStatus()
{
  vardisp2("I", "SSID: ", WiFi.SSID());
  vardisp2("I", "IP: ", WiFi.localIP());
}

