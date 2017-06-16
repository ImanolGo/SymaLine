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

char ssid[] = "TP-LINK_54E4"; //  your network SSID (name)
char pass[] = "27155332";    // your network password (use for WPA, or use as key for WEP)

IPAddress ip(192, 168, 0, 21); //  Dress IP
IPAddress gateway(192, 168, 0, 1); // set gateway to match your network
IPAddress subnet(255, 255, 255, 0); // set subnet mask to match your network
IPAddress ipRemote(192, 168, 0, 100); //Remote IP

unsigned int localPort = 2390;      // local port to listen on
WiFiUDP Udp;

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "acknowledged";       // a string to send back

const int BUTTON_PIN = 0;
const int LED_PIN = 5;

const int NUM_TOUCH_PINS = 9;
const int touchPins[] = {T0,T2,T3,T4,T5,T6,T7,T8,T9};

void setup()                    
{
  setupSerial();
  setupWifi();

}

void setupSerial() {
   //Initialize serial and wait for port to open:
  Serial.begin(115200);
  delay(100);
}

void setupWifi() {

  initializeWifi();
  
  // Print WiFi MAC address:
  printMacAddress();

  connectWifi();
  printWiFiStatus();
  
}



void initializeWifi() {
    WiFi.disconnect();
    Udp.stop();
    Udp.flush();
  
    WiFi.config(ip, gateway, subnet);
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

    Serial.print("\nStarting connection to UDP port ");
    Serial.println(localPort);
    // if you get a connection, report back via serial:
    Udp.begin(localPort);
    Udp.flush();
}

void loop()                    
{
   checkWifiConnection();
   sendTouchPins();
}

void checkWifiConnection(){
  if(WiFi.status() != WL_CONNECTED) {
      
      initializeWifi();
      connectWifi();
      printWiFiStatus();
   }
}


void sendTouchPins()
{
  
    // send a reply, to the IP address and port that sent us the packet we received
//    if( !Udp.beginPacket("imanolgo-pro.local", localPort)){
//       Serial.println("Could not resolve the hostname or port.");
//    }

    if( !Udp.beginPacket(ipRemote, localPort)){
       Serial.println("Could not resolve the hostname or port.");
    }
   
    //Udp.write(ReplyBuffer);
   
   for(int i = 0; i< NUM_TOUCH_PINS; i++)
   {
      //Serial.print(touchRead(touchPins[i]));  // get value using Ti
      //Serial.print(" ");
      Udp.print(touchRead(touchPins[i]));  // get value using Ti
      Udp.print(" ");
   }
    //Serial.println("");
    //delay(500);

    Udp.endPacket();
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






