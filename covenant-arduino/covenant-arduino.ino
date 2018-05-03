#include "WiFiEsp.h"
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3); // RX, TX
#endif


/* PROGRAM SETTINGS */
int threshold = 200;
char ssid[] = "retardis";
char pass[] = "sigepala";
char host[] = "192.168.43.21";
int port = 3000;
const unsigned long postingInterval = 10000L; //10000L = 10 seconds


/* MISC. GLOBAL VARIABLES */
int status = WL_IDLE_STATUS;
String macStr;
unsigned long lastConnectionTime = 0;

WiFiEspClient client;


/* FUNCTIONS */

void(* resetBoard) (void) = 0;


void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  WiFi.init(&Serial1);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    resetBoard();
  }
  
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  printWifiStatus();
}


void loop()
{
  /*while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }*/

  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }
}


void httpRequest()
{
  Serial.println();
  
  client.stop();

  if (client.connect(host, port)) {
    Serial.println("Sending data...");

    String postdata = String("{\"hardware_id\":\"");
    postdata.concat(String(macStr));
    postdata.concat(String("\", \"token\":\""));
    postdata.concat(String("blabla"));
    postdata.concat(String("\", \"analog_reading\":"));
    postdata.concat(String(analogRead(A0)));
    postdata.concat(String(", \"threshold\":"));
    postdata.concat(String(threshold));
    postdata.concat(String("}"));
    
    client.println(F("POST /api/v1/reading_controller HTTP/1.1"));
    client.print(F("Host: "));
    client.println(host);
    client.println(F("User-Agent: ACEduino/2.1"));
    client.println(F("Content-Type: application/json; charset=utf-8"));
    client.print(F("Content-Length: "));
    client.println(postdata.length());
    client.println(F("Connection: close"));
    client.println();
    client.println(postdata);

    Serial.println("Data sent");

    lastConnectionTime = millis();
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

  Serial.print("MAC:");
  Serial.println(macStr);
  
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Signal strength (RSSI):");
  Serial.print(WiFi.RSSI());
  Serial.println(" dBm");
}
