#include "WiFiEsp.h"
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3);
#define vardisp(x,y) \
  if (DEBUG_WIFI) {\
  Serial.print("[");\
  Serial.print(x);\
  Serial.print("] ");\
  Serial.println(y);}
#define vardisp2(x,y,z) \
  if (DEBUG_WIFI) {\
  Serial.print("[");\
  Serial.print(x);\
  Serial.print("] ");\
  Serial.print(y);\
  Serial.println(z);}


/* PROGRAM SETTINGS */
int threshold = 200;
char ssid[] = "retardis";
char pass[] = "sigepala";
char host[] = "192.168.43.21";
int port = 3000;
const unsigned long postingInterval = 10000L; //10000L = 10 seconds
#define DEBUG_WIFI true


/* MISC. GLOBAL VARIABLES */
int status = WL_IDLE_STATUS;
String macStr;
unsigned long lastConnectionTime = 0;
int sensorVal;
WiFiEspClient client;


/* FUNCTIONS */

void(* resetBoard) (void) = 0;


void setup()
{
  Serial.begin(115200);
  while (!Serial);
  
  Serial1.begin(9600);  // ESP-12E only supports up to 9600 baudrate
  while (!Serial1);
  
  WiFi.init(&Serial1);

  if (WiFi.status() == WL_NO_SHIELD) {
    vardisp("ERR", "WiFi shield not present");
    resetBoard();
  }
  
  while ( status != WL_CONNECTED) {
    vardisp2("INF", "Connecting to SSID: ", ssid);
    status = WiFi.begin(ssid, pass);
  }

  vardisp2("INF", "Connected to SSID: ", ssid);
  printWifiStatus();
}


void loop()
{
  if (DEBUG_WIFI) {
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
  }

  if (millis() - lastConnectionTime > postingInterval) {
    sensorVal = analogRead(A0);
    vardisp2("INFO", "Sensor reading: ", sensorVal);
    
    if (WiFi.ping(host)) {
      httpRequest();
    }
    else {
      vardisp2("WRN", "Server ping failed ", millis()/1000);
    }
  }
}


void httpRequest()
{
  client.stop();
  vardisp("INF", "Connecting to server:");
  vardisp2("INF", "Hostname: ", host);
  vardisp2("INF", "Port: ", port);
  if (client.connect(host, port)) {
    vardisp("INF", "Connected, sending data to server");

    String postdata = String("{\"hardware_id\":\"");
    postdata.concat(String(macStr));
    postdata.concat(String("\", \"token\":\""));
    postdata.concat(String("blabla"));
    postdata.concat(String("\", \"analog_reading\":"));
    postdata.concat(String(sensorVal));
    postdata.concat(String(", \"threshold\":"));
    postdata.concat(String(threshold));
    postdata.concat(String("}"));

    client.println(F("POST /api/v1/reading#create HTTP/1.1"));
    client.print(F("Host: "));
    client.println(host);
    client.println(F("User-Agent: ACEduino/2.1"));
    client.println(F("Content-Type: application/json; charset=utf-8"));
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

