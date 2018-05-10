#include <EtherCard.h>
#define vardisp(x,y) \
  if (debug_cov) {\
  Serial.print(F("["));\
  Serial.print(F(x));\
  Serial.print(F("] "));\
  Serial.println(y);}
#define vardisp2(x,y,z) \
  if (debug_cov) {\
  Serial.print(F("["));\
  Serial.print(F(x));\
  Serial.print(F("] "));\
  Serial.print(y);\
  Serial.println(z);}


/* PROGRAM SETTINGS */
const char host[] PROGMEM = "immense-plateau-44759.herokuapp.com";
//const char host[] = "192.168.43.21";
const int port = 80;
const unsigned long postingInterval = 30000L; //1000L = 1 second
const unsigned long readInterval = 1000L;
const int threshold = 40;
bool usePing = false;
const bool debug_cov = true;

static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x31,0x45 };
const char macStr[] = "7469692D3145";


/* MISC. GLOBAL VARIABLES */
byte Ethernet::buffer[700];
static uint32_t timer;
Stash stash;

unsigned long lastConnectionTime = 0;
unsigned long lastReadTime = 0;
unsigned long lastTransTime = 0;
unsigned long lastResetTime = 0;
int sensorVal;
bool sensorOnly = false;
bool runningFan = false;
const int greenLED = 4;
const int yellowLED = 5;
const int orangeLED = 6;
const int redLED = 7;
// 2, 4, 6-minute intervals for LED indicators
const unsigned long ledTimes[] = {120000,240000,360000};
bool lastState = false;


/* FUNCTIONS */

// called when the client request is complete
static void printResp (byte status, word off, word len) {
  Serial.println(">>>");
  //Ethernet::buffer[off+500] = 0;
  Serial.print((const char*) Ethernet::buffer + off);
  Serial.println("...");
}

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
  Serial.println();
  vardisp("INF", "Serial port ready");

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) {
    vardisp("ERR", "Failed to access Ethernet controller");
    vardisp("WRN", "Board will now run in sensor-only mode");
    sensorOnly = true;
  }
  if (!ether.dhcpSetup()) {
    vardisp("ERR", "DHCP failed");
    vardisp("WRN", "Board will now run in sensor-only mode");
    sensorOnly = true;
  }

  ether.printIp("[ETH] IP:  ", ether.myip);
  ether.printIp("[ETH] GW:  ", ether.gwip);  
  ether.printIp("[ETH] DNS: ", ether.dnsip);  

  if (!ether.dnsLookup(host)) {
    vardisp("ERR", "DNS failed");
    vardisp("WRN", "Board will now run in sensor-only mode");
    sensorOnly = true;
  }
  
  ether.printIp("[ETH] SRV: ", ether.hisip);

  sensorVal = analogRead(A0);
  if (sensorVal >= threshold) {
    lastState = true;
    lastTransTime = millis();
  }
}


void loop()
{
  if (millis() - lastReadTime > readInterval) {
    sensorVal = analogRead(A0);
    vardisp2("INF", "Sensor reading: ", sensorVal);
    unsigned long t = millis() - lastTransTime;
    if (sensorVal >= threshold) {
      if (!lastState) {
        vardisp("INF", "Sensor level threshold reached, starting countdown");
        lastState = true;
        lastTransTime = millis();
        t = 0;
      }
      if (t <= ledTimes[0]){
        digitalWrite(greenLED,HIGH);
        digitalWrite(yellowLED,LOW);
        digitalWrite(orangeLED,LOW);
        digitalWrite(redLED,LOW);
      }
      else if (t > ledTimes[0] && t <= ledTimes[1]){
        digitalWrite(greenLED,LOW);
        digitalWrite(yellowLED,HIGH);
        digitalWrite(orangeLED,LOW);
        digitalWrite(redLED,LOW);
        if (!runningFan) {
          runningFan = true;
          vardisp("INF", "Turning on fan");
          digitalWrite(A5, HIGH);
        }
      }
      else if (t > ledTimes[1] && t <= ledTimes[2]){
        digitalWrite(greenLED,LOW);
        digitalWrite(yellowLED,LOW);
        digitalWrite(orangeLED,HIGH);
        digitalWrite(redLED,LOW);
      }
      else if (t > ledTimes[2]){
        digitalWrite(greenLED,LOW);
        digitalWrite(yellowLED,LOW);
        digitalWrite(orangeLED,LOW);
        digitalWrite(redLED,HIGH);
      }
    }
    else {
      if (lastState) {
        vardisp("INF", "Sensor level below threshold, stopping countdown");
        lastState = false;
        lastResetTime = millis();
      }
      unsigned long t2 = millis() - lastResetTime;
      if (t2 > ledTimes[0]) {
        if (runningFan) {
          runningFan = false;
          vardisp("INF", "Turning off fan");
          digitalWrite(A5, LOW);
          digitalWrite(greenLED,HIGH);
          digitalWrite(yellowLED,LOW);
          digitalWrite(orangeLED,LOW);
          digitalWrite(redLED,LOW);
        }
      }
    }
    lastReadTime = millis();
  }

  if (!sensorOnly) {
    if (debug_cov) {
      ether.packetLoop(ether.packetReceive());
    }
    else {
      
    }
    if (millis() - lastConnectionTime > postingInterval) {
      httpRequest(sensorVal);
      vardisp("INF", "done");
    }
  }
}


void httpRequest(int reading)
{
  vardisp("INF", "Connecting to server:");
  vardisp2("INF", "Hostname: ", host);
  vardisp2("INF", "Port: ", port);
  vardisp("INF", "Connected, sending data to server");

  byte postdata = stash.create();
  stash.print("{\"hardware_id\":");
  if (debug_cov) {
    stash.print("12345");
  }
  else {
    stash.print(macStr);
  }
  stash.print(",\"token\":");
  if (debug_cov) {
    stash.print("7397");
  }
  else {
    stash.print("145");
  }
  stash.print(",\"analog_reading\":");
  stash.print(reading);
  stash.print(",\"threshold\":");
  stash.print(threshold);
  stash.print("}");
  stash.save();
  
  Stash::prepare(PSTR("POST /api/v1/reading HTTP/1.1" "\r\n"
    "Host: $F" "\r\n"
    "Content-Length: $D" "\r\n"
    "Content-Type: application/json" "\r\n"
    "\r\n"
    "$H"), host, PSTR("api/v1/reading"), stash.size(), postdata);
  
  uint8_t sock = ether.tcpSend();
  vardisp("TCP", ether.tcpReply(sock));

  vardisp("INF", "Data sent to server");
  lastConnectionTime = millis();
}

