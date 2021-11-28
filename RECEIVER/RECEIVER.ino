#include <Wire.h> 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//#define DEBUG

RF24 radio(10, 9); // CE, CSN
const byte address[6] = "00001";

enum buttons {LYF, LYB, RXL, RXR, STOP, LIGHT, POWER_DOWN, POWER_UP};

enum slaves {HEART};

typedef struct {
  byte key;
  byte value;
} Package;

Package package;

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
  #endif
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setPayloadSize(2);
  radio.setChannel(2);
  radio.startListening();
  Wire.begin(); 
  #ifdef DEBUG
    Serial.println("I am Receiver");
  #endif
}

void loop() {
  if (radio.available()) {
      radio.read(&package, sizeof(package));
      #ifdef DEBUG
        Serial.print(package.key);
        Serial.println(package.value);
      #endif
      Wire.beginTransmission(slaves::HEART);
      Wire.write((byte *)&package, sizeof package);
      Wire.endTransmission();  
  } 
}
