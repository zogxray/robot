/*
  Arduino Wireless Communication Tutorial
      Example 1 - Transmitter Code

  by Dejan Nedelkovski, www.HowToMechatronics.com

  Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <PS4USB.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

USB Usb;
PS4USB PS4(&Usb);
RF24 radio(6, 5); // CE, CSN

int pulseLedInterval = 1000;
unsigned long previousMillisPulse;

const byte address[6] = "00001";

enum buttons {LY, RX};
enum dirs {LYF, LYB, RXL, RXR, STOP, LIGHT};

typedef struct {
  byte key;
  byte value;
} Package;

Package package;

ColorsEnum colors[6] = {Red, Green, Blue, Yellow, Lightblue, Purple};

void setup() {
  Serial.begin(115200);
  while (!Serial);

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  Serial.println("I am Transmitter");

  if (Usb.Init() == -1) {
    Serial.println(F("\r\nOSC did not start"));
    while (1); // Halt
  }

  Serial.println(F("\r\nPS4 USB Library Started"));
}

void loop() {
  Usb.Task();

  if (millis() - previousMillisPulse >= pulseLedInterval)
  {
    previousMillisPulse = millis();
    PS4.setLed(colors[random(0, 4)]);
    Serial.print(F("Tomet tick: "));
    Serial.println(colors[random(0, 4)]);
  }

  PS4.setRumbleOff();

  if (PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117) {
      Serial.print(F("\r\nLeftHatY: "));
      Serial.print(PS4.getAnalogHat(LeftHatY));

//      PS4.setRumbleOn(RumbleLow);
      
      if (PS4.getAnalogHat(LeftHatY) < 117) {
        package.key = dirs::LYF;
        package.value = map(PS4.getAnalogHat(LeftHatY), 0, 116, 255, 0);
      } else {
        package.key = dirs::LYB;
        package.value = map(PS4.getAnalogHat(LeftHatY), 138, 255, 0, 255);
      }
      
      radio.write(&package, sizeof(package));
  }
  
  if (PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117) {
      Serial.print(F("\r\nRightHatX: "));
      Serial.print(PS4.getAnalogHat(RightHatX));

//      PS4.setRumbleOn(RumbleHigh);

      if (PS4.getAnalogHat(RightHatX) < 117) {
        package.key = dirs::RXL;
        package.value = map(PS4.getAnalogHat(RightHatX), 0, 116, 255, 0);
      } else {
        package.key = dirs::RXR;
        package.value = map(PS4.getAnalogHat(RightHatX), 138, 255, 0, 255);
      }
      
      radio.write(&package, sizeof(package));
  }

  if (PS4.getButtonClick(CIRCLE)) {
      package.key = dirs::LIGHT;
      package.value = 0;
      radio.write(&package, sizeof(package));
  }
}
