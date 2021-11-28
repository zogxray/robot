#include <PS4USB.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

USB Usb;
PS4USB PS4(&Usb);
RF24 radio(6, 5); // CE, CSN

//#define DEBUG

int pulseLedInterval = 1000;
unsigned long previousMillisPulse;

const byte address[6] = "00001";


enum buttons {
    LYF,
    LYB,
    RXL,
    RXR,
    STOP,
    LIGHT,
    POWER_DOWN,
    POWER_UP,
    TRIM_WHEEL_LEFT,
    TRIM_WHEEL_RIGHT,
    PING,
    PONG
  };

typedef struct {
  byte key;
  byte value;
} Package;

Package package;

ColorsEnum colors[6] = {Red, Green, Blue, Yellow, Lightblue, Purple};

void setup() {
  #ifdef DEBUG
    Serial.begin(115200);
    while (!Serial);
  #endif

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_250KBPS);
  radio.setPayloadSize(2);
  radio.setChannel(2);
  radio.stopListening();

  #ifdef DEBUG
    Serial.println("I am Transmitter");
  #endif


  if (Usb.Init() == -1) {
    #ifdef DEBUG
        Serial.println(F("\r\nOSC did not start"));
    #endif
    while (1); // Halt
  }

  #ifdef DEBUG
    Serial.println(F("\r\nPS4 USB Library Started"));
  #endif
}

void loop() {
  Usb.Task();

  if (PS4.connected()) {
    if (millis() - previousMillisPulse >= pulseLedInterval)
    {
      previousMillisPulse = millis();
      PS4.setLed(colors[random(0, 4)]);
      #ifdef DEBUG
        Serial.print(F("Timer tick: "));
        Serial.println(colors[random(0, 4)]);
      #endif
      package.key = buttons::PING ;
      package.value = 0;
      radio.write(&package, sizeof(package));
    }
   
    if (PS4.getAnalogHat(LeftHatY) > 137 || PS4.getAnalogHat(LeftHatY) < 117) {
        #ifdef DEBUG
          Serial.print(F("\r\nLeftHatY: "));
          Serial.print(PS4.getAnalogHat(LeftHatY));
        #endif
        if (PS4.getAnalogHat(LeftHatY) < 117) {
          package.key = buttons::LYF;
          package.value = map(PS4.getAnalogHat(LeftHatY), 0, 116, 255, 0);
        } else {
          package.key = buttons::LYB;
          package.value = map(PS4.getAnalogHat(LeftHatY), 138, 255, 0, 255);
        }
        
        radio.write(&package, sizeof(package));
    } else {
       package.key = buttons::LYF;
       package.value = 0;
       radio.write(&package, sizeof(package));
       package.key = buttons::LYB;
       package.value = 0;
       radio.write(&package, sizeof(package));
    }
    
    if (PS4.getAnalogHat(RightHatX) > 137 || PS4.getAnalogHat(RightHatX) < 117) {
        #ifdef DEBUG
          Serial.print(F("\r\nRightHatX: "));
          Serial.print(PS4.getAnalogHat(RightHatX));
        #endif
        if (PS4.getAnalogHat(RightHatX) < 117) {
          package.key = buttons::RXL;
          package.value = map(PS4.getAnalogHat(RightHatX), 0, 116, 255, 0);
        } else {
          package.key = buttons::RXR;
          package.value = map(PS4.getAnalogHat(RightHatX), 138, 255, 0, 255);
        }
        
        radio.write(&package, sizeof(package));
    } else {
       package.key = buttons::RXL;
       package.value = 0;
       radio.write(&package, sizeof(package));
       package.key = buttons::RXR;
       package.value = 0;
       radio.write(&package, sizeof(package));
    }
  
    if (PS4.getButtonClick(CIRCLE)) {
        #ifdef DEBUG
          Serial.print(F("\r\nLIGHT"));
        #endif
        package.key = buttons::LIGHT;
        package.value = 0;
        radio.write(&package, sizeof(package));
    }

    if (PS4.getButtonClick(SHARE)) {
      #ifdef DEBUG
        Serial.print(F("\r\nPOWER_DOWN"));
      #endif
      package.key = buttons::TRIM_WHEEL_LEFT;
      package.value = 0;
      radio.write(&package, sizeof(package));
    }
      
    if (PS4.getButtonClick(OPTIONS)) {
      #ifdef DEBUG
        Serial.print(F("\r\nPOWER_UP"));
      #endif
      package.key = buttons::TRIM_WHEEL_RIGHT;
      package.value = 0;
      radio.write(&package, sizeof(package));
    }

    if (PS4.getButtonClick(L1)) {
      Serial.print(F("\r\nTRIM_WHEEL_LEFT"));
      package.key = buttons::POWER_DOWN;
      package.value = 0;
      radio.write(&package, sizeof(package));
    }

    if (PS4.getButtonClick(R1)) {
      Serial.print(F("\r\nTRIM_WHEEL_RIGHT"));
      package.key = buttons::POWER_UP;
      package.value = 0;
      radio.write(&package, sizeof(package));
    }
  }
}
