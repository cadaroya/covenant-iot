#include "WiFiEsp.h"
#include "SoftwareSerial.h"
#define vardisp(x,y) \
  if (debug_cov) {\
  Serial.print(F("["));\
  Serial.print(x);\
  Serial.print(F("] "));\
  Serial.println(y);}
#define vardisp2(x,y,z) \
  if (debug_cov) {\
  Serial.print(F("["));\
  Serial.print(x);\
  Serial.print(F("] "));\
  Serial.print(y);\
  Serial.println(z);}
SoftwareSerial Serial1(2,3);


/* TODO
 *  - 2-4-6 min. lighting
 *    - green: no CO / safe
 *    - yellow: CO present, start timer: if past 2 min and >,
 *    - orange: CO present, if past 2 min and >,
 *    - red: CO present, alarm
 */


/* PROGRAM SETTINGS */
/*const char ssid[] = "cs131";
const char pass[] = "urd1(31)4me<3";*/
const char ssid[] = "retardis";
const char pass[] = "sigepala";
const char host[] = "immense-plateau-44759.herokuapp.com";
const int port = 80;
const unsigned long postingInterval = 60000L; //1000L = 1 second
const unsigned long readInterval = 500L;
const unsigned long threshTimeout = 30000L;
const int threshold = 100;
const bool debug_cov = true;


/* MISC. GLOBAL VARIABLES */
bool usePing = false;
int wifiStatus = WL_IDLE_STATUS;
String macStr;
unsigned long lastConnectionTime = 0;
unsigned long lastReadTime = 0;
unsigned long lastThreshTime = 0;
unsigned long lastTransTime = 0;
int sensorVal;
bool sensorOnly = false;
bool runningFan = false;
WiFiEspClient client;
const int greenLED = 4;
const int yellowLED = 5;
const int orangeLED = 6;
const int redLED = 7;


/* FUNCTIONS */

void setup()
{
  pinMode(A0, INPUT);
  pinMode(A5, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(orangeLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  
  Serial.begin(9600);
  while (!Serial);
  Serial1.begin(9600);  // ESP-12E only supports up to 9600 baud
  while (!Serial1);
  WiFi.init(&Serial1);

  if (WiFi.status() == WL_NO_SHIELD) {
    vardisp("ERR", "WiFi shield not present");
    vardisp("WRN", "Board will now run in sensor-only mode");
    sensorOnly = true;
  }

  if (!sensorOnly) {
    while ( wifiStatus != WL_CONNECTED) {
      vardisp2("INF", "Connecting to SSID: ", ssid);
      wifiStatus = WiFi.begin(ssid, pass);
    }
    vardisp2("INF", "Connected to SSID: ", ssid);
    printWifiStatus();
  }
}


void loop()
{
  sensorVal = analogRead(A0);

  if (debug_cov) {
    if (millis() - lastReadTime > readInterval) {
      vardisp2("INF", "Sensor reading: ", sensorVal);
      lastReadTime = millis();
    }
  }
  
  if (sensorVal > threshold && !runningFan && millis() - lastThreshTime > threshTimeout) {
    vardisp("INF", "Sensor level threshold reached, turning on fan");
    digitalWrite(A5, HIGH);
    lastThreshTime = millis();
    runningFan = true;
  }
  else if (sensorVal <= threshold && runningFan && millis() - lastThreshTime > threshTimeout) {
    vardisp("INF", "Sensor level below threshold for some time, turning off fan");
    digitalWrite(A5, LOW);
    lastThreshTime = millis();
    runningFan = false;
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
          httpRequest(sensorVal);
        }
        else {
          vardisp2("WRN", "Server ping failed ", millis()/1000);
        }
      }
      else {
        httpRequest(sensorVal);
      }
    }
  }
  if (sensorVal <= 70 && sensorVal > 0){
    digitalWrite(greenLED,HIGH);
    digitalWrite(yellowLED,LOW);
    digitalWrite(orangeLED,LOW);
    digitalWrite(redLED,LOW);
  }
  else if (sensorVal <= 90 && sensorVal > 70){
    digitalWrite(greenLED,LOW);
    digitalWrite(yellowLED,HIGH);
    digitalWrite(orangeLED,LOW);
    digitalWrite(redLED,LOW);
  }
  else if (sensorVal <= threshold && sensorVal > 90){
    digitalWrite(greenLED,LOW);
    digitalWrite(yellowLED,LOW);
    digitalWrite(orangeLED,HIGH);
    digitalWrite(redLED,LOW);
  }
  else if (sensorVal > threshold){
    digitalWrite(greenLED,LOW);
    digitalWrite(yellowLED,LOW);
    digitalWrite(orangeLED,LOW);
    digitalWrite(redLED,HIGH);    
  }
}


void httpRequest(int reading)
{
  client.stop();
  vardisp("INF", "Connecting to server:");
  vardisp2("INF", "Hostname: ", host);
  vardisp2("INF", "Port: ", port);
  if (client.connect(host, port)) {
    vardisp("INF", "Connected, sending data to server");

    String postdata = String(String("{\"analog_reading\":") + reading + ", \"threshold\":" + threshold + "}");

    client.print(F("POST /api/v1/"));
    if (debug_cov) {
      client.print(F("12345"));
    }
    else {
      client.print(macStr);
    }
    client.println(F("/"));
    if (debug_cov) {
      client.print(F("7397"));
    }
    else {
      client.print(F("145"));
    }
    client.println(F("HTTP/1.1"));
    client.print(F("Host: "));
    client.println(host);
    client.println(F("User-Agent: ACEduino/2.1"));
    client.println(F("Content-Type: application/json"));
    client.print(F("Content-Length: "));
    client.println(postdata.length());
    client.println(F("Connection: close"));
    client.println();
    client.println(postdata);

    vardisp("INF", "Data sent to server");
    lastConnectionTime = millis();
  }
  else {
    vardisp("WRN", "Connection failed");
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
  vardisp2("INF", "MAC:", macStr);
  vardisp2("INF", "SSID: ", WiFi.SSID());
  vardisp2("INF", "IP Address: ", WiFi.localIP());
  vardisp2("INF", "RSSI: ", WiFi.RSSI());
}

