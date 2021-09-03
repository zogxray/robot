
#include <Wire.h> 
//#define DEBUG

//MAIN DRIVE
const int DRIVE_DIR = 2;
const int DRIVE_PWM = 3;

const int LEFT_LIGHT_HB = 5;
const int RIGHT_LIGHT_HB = 6;

//WHEEL DRIVE
const int WHEEL_DIR = 12;
const int WHEEL_PVM = 10;

const int FORWARD_LIGHT_HB_FUN = 4;
const int FORWARD_LIGHT_HB = 9;

enum buttons {LY, RX};
enum dirs {LYF, LYB, RXL, RXR, STOP, LIGHT};
enum slaves {HEART};

typedef struct {
  byte key;
  byte value;
} Package;

Package package;

int stopCount = 0;
int stopFunCount = 0;
int ledState = 0;

const int WHEEL_DIVIDER = 12;


void setup() { 
  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("I am heart");
  #endif

  pinMode(DRIVE_PWM, OUTPUT);
  pinMode(DRIVE_DIR, OUTPUT);

  pinMode(WHEEL_DIR, OUTPUT);
  pinMode(WHEEL_PVM, OUTPUT);

  pinMode(LEFT_LIGHT_HB, OUTPUT);
  pinMode(RIGHT_LIGHT_HB, OUTPUT);

  pinMode(FORWARD_LIGHT_HB_FUN, OUTPUT);
  pinMode(FORWARD_LIGHT_HB, OUTPUT);

  Wire.begin(slaves::HEART); 
  Wire.onReceive(receiveEvent);
}

void receiveEvent(int bytes) {
    Wire.readBytes((byte*)&package, sizeof(package));

    if (package.key == dirs::LIGHT) {
      ledState += 1;
      if (ledState > 4) {
        ledState = 0;
      }
    }
    
    if (package.key == dirs::STOP) {
        stopCount += 1;
        if (stopCount > 7) {
          #ifdef DEBUG
            Serial.print("STOP: ");
            Serial.println(package.value);
          #endif
          analogWrite(DRIVE_PWM,0);
          digitalWrite(DRIVE_DIR, LOW);
          analogWrite(WHEEL_PVM, 0);
          digitalWrite(WHEEL_DIR, LOW);

          analogWrite(LEFT_LIGHT_HB, 0);
          analogWrite(RIGHT_LIGHT_HB, 0);

          stopCount = 0;
        }
    }

   if (package.key == dirs::STOP) {
        stopFunCount += 1;
        if (stopFunCount > 15000) {
          #ifdef DEBUG
            Serial.print("STOP FUN: ");
            Serial.println(package.value);
          #endif
          stopFunCount = 0;
        }
    }
      
    if (package.key == dirs::LYF) {
        #ifdef DEBUG
          Serial.print("LYF: ");
          Serial.println(package.value);
        #endif
        analogWrite(DRIVE_PWM, package.value);
        digitalWrite(DRIVE_DIR, LOW);
        stopCount = 0;
    }

    if (package.key == dirs::LYB) {
        #ifdef DEBUG
          Serial.print("LYB: ");
          Serial.println(package.value);
        #endif
        analogWrite(DRIVE_PWM, package.value);
        digitalWrite(DRIVE_DIR, HIGH);
        stopCount = 0;
    }

    if (package.key == dirs::RXL) {
        #ifdef DEBUG
          Serial.print("RXL: ");
          Serial.println(package.value);
        #endif
        analogWrite(WHEEL_PVM, package.value/WHEEL_DIVIDER);
        digitalWrite(WHEEL_DIR, LOW);
        
        analogWrite(LEFT_LIGHT_HB, 64);
        
        stopCount = 0;
        stopFunCount = 0;
    }

   if (package.key == dirs::RXR) {
        #ifdef DEBUG
          Serial.print("RXR: ");
          Serial.println(package.value);
        #endif
        analogWrite(WHEEL_PVM, package.value/WHEEL_DIVIDER);
        digitalWrite(WHEEL_DIR, HIGH);
  
        analogWrite(RIGHT_LIGHT_HB, 64);
        
        stopCount = 0;
        stopFunCount = 0;
    }
}

void loop() {
     switch (ledState) {
          case 1:
            analogWrite(FORWARD_LIGHT_HB, 8);
            digitalWrite(FORWARD_LIGHT_HB_FUN, HIGH);
            break;
          case 2:
            analogWrite(FORWARD_LIGHT_HB, 16);
            digitalWrite(FORWARD_LIGHT_HB_FUN, HIGH);
            break;
          case 3:
            analogWrite(FORWARD_LIGHT_HB, 32);
            digitalWrite(FORWARD_LIGHT_HB_FUN, HIGH);
            break;
          case 4:
            analogWrite(FORWARD_LIGHT_HB, 64);
            digitalWrite(FORWARD_LIGHT_HB_FUN, HIGH);
            break;      
          default:
             digitalWrite(FORWARD_LIGHT_HB_FUN, LOW);
             analogWrite(FORWARD_LIGHT_HB, 0);
        }
}
