/*
 WiFiEsp example: WebClientRepeating

 This sketch connects to a web server and makes an HTTP request
 using an Arduino ESP8266 module.
 It repeats the HTTP call each 10 seconds.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
*/

#include "WiFiEsp.h"

// Emulate Serial1 on pins 6/7 if not present
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial Serial1(2, 3); // RX, TX
#endif

int threshold = 200;              // calibrate this

char ssid[] = "retardis";            // your network SSID (name)
char pass[] = "sigepala";        // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
byte mac[6];
String macStr;

char server[] = "192.168.43.21";
int port = 3000;

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000L; // delay between updates, in milliseconds

// Initialize the Ethernet client object
WiFiEspClient client;

void setup()
{
  // initialize serial for debugging
  Serial.begin(9600);
  // initialize serial for ESP module
  Serial1.begin(9600);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue
    while (true);
  }
  
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("You're connected to the network");
  
  printWifiStatus();
}

void loop()
{
  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if 10 seconds have passed since your last connection,
  // then connect again and send data
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }
}

// this method makes a HTTP connection to the server
void httpRequest()
{
  Serial.println();
    
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection
  if (client.connect(server, port)) {
    Serial.println("Connecting...");

    String postdata = String("{\"hardware_id\":\"");
    postdata.concat(String(macStr));
    postdata.concat(String("\", \"token\":\""));
    postdata.concat(String("blabla"));         // what's "token"?
    postdata.concat(String("\", \"analog_reading\":"));
    postdata.concat(String(analogRead(A0)));
    postdata.concat(String(", \"threshold\":"));
    postdata.concat(String(threshold));
    postdata.concat(String("}"));
    
    /* send the HTTP PUT request
    client.println(F("GET /asciilogo.txt HTTP/1.1"));
    client.println(F("Host: arduino.cc"));
    client.println("Connection: close");
    client.println();*/
    client.println(F("POST /api/v1/reading_controller HTTP/1.0"));
    client.print(F("Host: "));
    client.println(server);
    client.println(F("User-Agent: ACEduino/2.1"));
    client.println(F("Content-Type: application/json; charset=utf-8"));
    client.print(F("Content-Length: "));
    client.println(postdata.length());
    client.println(F("Connection: close"));
    client.println();
    client.println(postdata);

    // note the time that the connection was made
    lastConnectionTime = millis();
  }
  else {
    // if you couldn't make a connection
    Serial.println("Connection failed");
  }
}


void printWifiStatus()
{
  WiFi.macAddress(mac);
  macStr = String();
  for (int i=5; i>=0; i--) {
    macStr.concat(String(int(mac[i]), HEX));
  }

  Serial.print("MAC:");
  Serial.println(macStr);
  
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
