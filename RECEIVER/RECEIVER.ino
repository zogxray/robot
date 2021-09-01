/*
* Arduino Wireless Communication Tutorial
*       Example 1 - Receiver Code
*                
* by Dejan Nedelkovski, www.HowToMechatronics.com
* 
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <Wire.h> 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(10, 9); // CE, CSN
const byte address[6] = "00001";

enum buttons {LY, RX};
enum dirs {LYF, LYB, RXL, RXR, STOP};
enum slaves {HEART};

typedef struct {
  byte key;
  byte value;
} Package;

Package package;

void setup() {
  //Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  Wire.begin(); 
  //Serial.println("I am Receiver");
}

void loop() {
  if (radio.available()) {
      radio.read(&package, sizeof(package));
    
//    if (package.key == dirs::LYF) {
//        Serial.print("LYF: ");
//        Serial.println(package.value);
//    }
//
//    if (package.key == dirs::LYB) {
//        Serial.print("LYB: ");
//        Serial.println(package.value);
//    }
//
//    if (package.key == dirs::RXL) {
//        Serial.print("RXL: ");
//        Serial.println(package.value);
//    }
//
//    if (package.key == dirs::RXR) {
//        Serial.print("RX: ");
//        Serial.println(package.value);
//    }
    
      Wire.beginTransmission(slaves::HEART);
      Wire.write((byte *)&package, sizeof package);
      Wire.endTransmission();  
  }

  Wire.beginTransmission(slaves::HEART);
  package.key = dirs::STOP;
  package.value = 0;
  Wire.write((byte *)&package, sizeof package);
  Wire.endTransmission();  
}
