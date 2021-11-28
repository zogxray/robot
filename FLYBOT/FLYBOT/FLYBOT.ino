#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

//#define DEBUG
#define servoPin1 9
#define servoPin2 10
#define dirPin 2
#define pwmPin 3

Servo myservo1;
Servo myservo2;

RF24 radio(6, 5); // CE, CSN
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

int power = 0;
int powerStep = 64;
bool pingLed = false;
bool arm = false;
bool disarm = true;
int pulseInterval = 1000;
unsigned long previousMillisPulse;

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
  #ifdef DEBUG
    Serial.println("I am Receiver");
  #endif  

  myservo1.attach(servoPin1);
  myservo1.write(90);
  myservo2.attach(servoPin2);
  myservo2.write(90);

  pinMode(dirPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
}

void loop() { 
  #ifdef DEBUG
     Serial.print(F("Disarm: "));
     Serial.println(disarm);
  #endif
  
  if (disarm == true) {
      power = 0;
  } 
      
  analogWrite(pwmPin, power);
  
  digitalWrite(dirPin, HIGH);

  if (millis() - previousMillisPulse >= pulseInterval) {
      previousMillisPulse = millis();
      #ifdef DEBUG
        Serial.print(F("Timer tick: "));
      #endif
      if (arm == false) {
        disarm = true;
      }
      
      arm = false;
  }
  
  if (radio.available()) {
      radio.read(&package, sizeof(package));
      
      if (package.key == buttons::PING) {
         #ifdef DEBUG
            Serial.print("PING: ");
            Serial.println(pingLed);
         #endif
         arm = true; 
         disarm = false;   
      }

      if (package.key == buttons::LYF) {
         #ifdef DEBUG
            Serial.print("LYF: ");
            Serial.println(package.value);
         #endif
         myservo1.write(map(package.value, 0, 255, 90, 30));
      }
    
      if (package.key == buttons::LYB) {
          #ifdef DEBUG
            Serial.print("LYB: ");
            Serial.println(package.value);
          #endif
          myservo1.write(map(package.value, 0, 255, 90, 140));
      }
    
      if (package.key == buttons::RXL) {
        #ifdef DEBUG
            Serial.print("RXL: ");
            Serial.println(package.value);
        #endif
        myservo2.write(map(package.value, 0, 255, 90, 30));
      }
    
      if (package.key == buttons::RXR) {
        #ifdef DEBUG
          Serial.print("RX: ");
          Serial.println(package.value);
        #endif
        myservo2.write(map(package.value, 0, 255, 90, 140));
      }
      
      if (package.key == buttons::POWER_DOWN) {
        #ifdef DEBUG
          Serial.print(F("POWER DOWN: "));
          Serial.println(power);
        #endif
        power -= powerStep;
        if (power < 0) 
        {
          power = 0;
        }
      }
        
      if (package.key == buttons::POWER_UP) {
        #ifdef DEBUG
          Serial.print(F("POWER UP: "));
          Serial.println(power);
        #endif
        power += powerStep;
        if (power > 255) 
        {
          power = 255;
        }
      }
  }
}
