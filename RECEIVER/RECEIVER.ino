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
enum dirs {LYF, LYB, RXL, RXR, STOP, LIGHT};
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
