/*
  Musical Touch Wifi
   
 Description:
 * It reads from the analog pins as capacitive sensors. 
 * It sends it then as a UDP array

 Software:
 * Capacitive Library CapSense by Paul Badger  
 
 Hardware:
* SparkFun ESP32 Thing 
* Conductive Fabric
* Conductive Thread
   
 created 16 June 2017
 by Imanol Gómez
 
 */

 
#include <WiFi.h>
#include <WiFiUdp.h>

//char ssid[] = "TP-LINK_2BDE"; //  your network SSID (name)
//char pass[] = "98639243";    // your network password (use for WPA, or use as key for WEP)

char ssid[] = "TP-LINK_54E4"; //  your network SSID (name)
char pass[] = "27155332";    // your network password (use for WPA, or use as key for WEP)


//IPAddress ip(192, 168, 0, 21); //  Dress IP
IPAddress ip(192, 168, 0, 22); //  Dress IP
IPAddress gateway(192, 168, 0, 1); // set gateway to match your network
IPAddress subnet(255, 255, 0, 0); // set subnet mask to match your network
IPAddress ipRemote(192, 168, 0, 100); //Remote IP
IPAddress ipBroadCast(192, 168, 0, 255); //Remote IP

//unsigned int sendPort = 2390;      // local port to send to
unsigned int sendPort = 2391;      //  port to send to
unsigned int receivePort = 3333;      // local port to receive 

WiFiUDP udp;

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

const int BUTTON_PIN = 0;
const int LED_PIN = 5;

const int NUM_TOUCH_PINS = 8;
const int touchPins[] = {T0,T3,T4,T5,T6,T7,T8,T9};

boolean connected = true;

void setup()                    
{
  setupSerial();
  setupWifi();
}

void setupSerial() {
   //Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(100);
  Serial.println("Starting Software!!");
}

void setupWifi() {

 //initializeWifi();
//  
//  // Print WiFi MAC address:
//  printMacAddress();
//
  //connectWifi();
//  printWiFiStatus();

  connectToWiFi(ssid, pass);
  
}

void initializeWifi() {
   
   // udp.stop();
    //udp.flush();
    WiFi.disconnect(true);
    WiFi.config(ip, gateway, subnet);
    WiFi.setAutoReconnect(true);
}


void connectWifi() {

  int ledState = 0;
     // attempt to connect to WiFi network:
   Serial.print("Attempting to connect to SSID: ");
   Serial.println(ssid);


  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) 
  {
    // Blink LED while we're connecting:
    digitalWrite(LED_PIN, ledState);
    ledState = (ledState + 1) % 2; // Flip ledState
    delay(500);
    Serial.print(".");
  }
 
   Serial.print("\nConnected to SSID: ");
   Serial.println(ssid);

   connected = true;

    //Serial.print("\nStarting connection to UDP port ");
    //Serial.println(receivePort);
    // if you get a connection, report back via serial:
    //udp.begin(receivePort);
    //udp.flush();
}

void loop()                    
{
   checkConnect();
   //checkWifiConnection();
   sendTouchPins();
}

void checkWifiConnection(){
  if(WiFi.status() != WL_CONNECTED) {
      Serial.print("Wifi Disconnected!!!");
      initializeWifi();
      connectWifi();
      printWiFiStatus();
   }
}


void checkConnect(){
 // if there's data available, read a packet
  int packetSize = udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    Serial.print(ipRemote);
    Serial.print(", port ");
    Serial.println(udp.remotePort());

    // read the packet into packetBufffer
    int len = udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }

    Serial.println("Contents:");
    Serial.println(packetBuffer);

    if(packetBuffer[0] == 'c'){
      ipRemote = udp.remoteIP();
       Serial.print("Sending to IP address: ");
       Serial.println(ipRemote);  
    }
    
  }
}


void sendTouchPins()
{
      //only send data when connected
    if(connected)
    {
       
//       if( !udp.beginPacket(ipRemote, sendPort)){
//       Serial.println("Could not resolve the hostname or port.");
//         }
    
      //Send a packet

      udp.beginPacket(ipRemote,sendPort);
       for(int i = 0; i< NUM_TOUCH_PINS; i++)
      {
         //Serial.print(touchRead(touchPins[i]));  // get value using Ti
         // Serial.print(" ");
          udp.print(touchRead(touchPins[i]));  // get value using Ti
          udp.print(" ");
      }  
      udp.endPacket();
    }

    delay(1);
}



void printMacAddress() {
  // the MAC address of your WiFi shield
  byte mac[6];

  // print your MAC address:
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void connectToWiFi(const char * ssid, const char * pwd){
  Serial.println("Connecting to WiFi network: " + String(ssid));

  // delete old config
  WiFi.disconnect(true);
  delay(100);
  //register event handlerpin
  WiFi.onEvent(WiFiEvent);
  WiFi.config(ip, gateway, subnet);
  WiFi.setAutoReconnect(true);
  WiFi.setAutoConnect(true);
  
  //Initiate connection
  WiFi.begin(ssid, pwd);

  Serial.println("Waiting for WIFI connection...");
}

//wifi event handler
void WiFiEvent(WiFiEvent_t event){
    switch(event) {
      case SYSTEM_EVENT_STA_GOT_IP:
          //When connected set 
          Serial.print("WiFi connected! IP address: ");
          Serial.println(WiFi.localIP());  
          //initializes the UDP state
          //This initializes the transfer buffer
          udp.begin(WiFi.localIP(),receivePort);
          connected = true;
          break;
      case SYSTEM_EVENT_STA_DISCONNECTED:
          Serial.println("WiFi lost connection");
          connected = false;
          software_Reset();
          connectToWiFi(ssid, pass);
          break;
    }
}

void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
  esp_restart();  //tells the SDK to reboot, so its a more clean reboot, use this one if possible.
}  




