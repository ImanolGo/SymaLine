/*
  Musical Touch
   
 Description:
 * It reads from the analog pins as capacitive sensors. It sends it then as an array

 Software:
 * Capacitive Library CapSense by Paul Badger  
 
 Hardware:
* Adafruit Circuit Playground
* Conductive Fabric
   
 created 12 April 2017
 by Imanol Gómez
 
 */

#include <CapPin.h>

#define NUM_PINS 8
 
CapPin cpins[NUM_PINS] = {CapPin(0), CapPin(1), CapPin(2), CapPin(3), CapPin(6), CapPin(9), CapPin(10), CapPin(12)};

float smoothed;

void setup()                    
{

  Serial.begin(115200);
  Serial.println("start");

}

void loop()                    
{
    for(int i=0; i<NUM_PINS; i++){
        Serial.print(cpins[i].readPin(100));
        Serial.print(" ");
    }
    Serial.println("");
}
