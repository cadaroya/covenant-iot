#include "WiFiEsp.h"
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3);
#define vardisp(x,y) \
  if (DEBUG_COV) {\
  Serial.print(F("["));\
  Serial.print(x);\
  Serial.print(F("] "));\
  Serial.println(y);}
#define vardisp2(x,y,z) \
  if (DEBUG_COV) {\
  Serial.print(F("["));\
  Serial.print(x);\
  Serial.print(F("] "));\
  Serial.print(y);\
  Serial.println(z);}


/* PROGRAM SETTINGS */
int threshold = 100;
char ssid[] = "retardis";
char pass[] = "sigepala";
char host[] = "192.168.43.21";
int port = 3000;
const unsigned long postingInterval = 60000L; //1000L = 1 second
const unsigned long readInterval = 3000L;
const unsigned long threshTimeout = 30000L;
#define DEBUG_COV true


/* MISC. GLOBAL VARIABLES */
int status = WL_IDLE_STATUS;
String macStr;
unsigned long lastConnectionTime = 0;
unsigned long lastReadTime = 0;
unsigned long lastThreshTime = 0;
int sensorVal;
bool sensorOnly = false;
bool runningFan = false;
WiFiEspClient client;


/* FUNCTIONS */

void setup()
{
  pinMode(A0, INPUT);
  pinMode(A5, OUTPUT);
  
  Serial.begin(9600);
  while (!Serial);
  Serial1.begin(9600);  // ESP-12E only supports up to 9600 baudrate
  while (!Serial1);
  WiFi.init(&Serial1);

  if (WiFi.status() == WL_NO_SHIELD) {
    vardisp("ERR", "WiFi shield not present");
    vardisp("WRN", "Board will now run in sensor-only mode");
    sensorOnly = true;
  }

  if (!sensorOnly) {
    while ( status != WL_CONNECTED) {
      vardisp2("INF", "Connecting to SSID: ", ssid);
      status = WiFi.begin(ssid, pass);
    }
    vardisp2("INF", "Connected to SSID: ", ssid);
    printWifiStatus();
  }
}


void loop()
{
  sensorVal = analogRead(A0);

  if (DEBUG_COV) {
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
    if (DEBUG_COV) {
      while (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
    }
    if (millis() - lastConnectionTime > postingInterval) {
      if (WiFi.ping(host)) {
        httpRequest(sensorVal);
      }
      else {
        vardisp2("WRN", "Server ping failed ", millis()/1000);
      }
    }
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

    String postdata = String("{\"hardware_id\":\"" + macStr +\
      "\", \"token\":\"" + "covenant-sample" +\
      "\", \"analog_reading\":" + reading +\
      ", \"threshold\":" + threshold + "}");

    client.println(F("POST /api/v1/reading HTTP/1.1"));
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

