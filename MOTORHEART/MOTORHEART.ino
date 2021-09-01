
#include <Wire.h> 

//MAIN DRIVE
const int DRIVE_DIR = 2;
const int DRIVE_PWM = 3;

//const int DIGITAL_INPUT_DRIVE_F = 6;
//const int DIGITAL_INPUT_DRIVE_B = 5;

//WHEEL DRIVE
const int WHEEL_DIR = 9;
const int WHEEL_PVM = 10;

//const int DIGITAL_INPUT_WHEEL_DIR = 7;          
//const int DIGITAL_INPUT_WHEEL_PVM = 8;

const int SYSTEM_FUN = 4;

enum buttons {LY, RX};
enum dirs {LYF, LYB, RXL, RXR, STOP};
enum slaves {HEART};

typedef struct {
  byte key;
  byte value;
} Package;

Package package;

int stopCount = 0;
int stopFunCount = 0;

const int WHEEL_DIVIDER = 12;


void setup() { 
  Serial.begin(115200);

  pinMode(DRIVE_PWM, OUTPUT);
  pinMode(DRIVE_DIR, OUTPUT);

//  pinMode(DIGITAL_INPUT_DRIVE_F, INPUT_PULLUP);
//  pinMode(DIGITAL_INPUT_DRIVE_B, INPUT_PULLUP);

  pinMode(WHEEL_DIR, OUTPUT);
  pinMode(WHEEL_PVM, OUTPUT);

//  pinMode(DIGITAL_INPUT_WHEEL_DIR, INPUT_PULLUP);
//  pinMode(DIGITAL_INPUT_WHEEL_PVM, INPUT_PULLUP);

  pinMode(SYSTEM_FUN, OUTPUT);

  Wire.begin(slaves::HEART); 
  Wire.onReceive(receiveEvent);
//
  Serial.println("I am heart");
}

void receiveEvent(int bytes) {
    Wire.readBytes((byte*)&package, sizeof(package));

    if (package.key == dirs::STOP) {
        stopCount += 1;
        if (stopCount > 7) {
//          Serial.print("STOP: ");
//          Serial.println(package.value);
          analogWrite(DRIVE_PWM,0);
          digitalWrite(DRIVE_DIR, LOW);
          analogWrite(WHEEL_PVM, 0);
          digitalWrite(WHEEL_DIR, LOW);
          stopCount = 0;
        }
    }

   if (package.key == dirs::STOP) {
        stopFunCount += 1;
        if (stopFunCount > 15000) {
          Serial.print("STOP FUN: ");
          Serial.println(package.value);
          digitalWrite(SYSTEM_FUN, LOW);
          stopFunCount = 0;
        }
    }
      
    if (package.key == dirs::LYF) {
        Serial.print("LYF: ");
        Serial.println(package.value);
        analogWrite(DRIVE_PWM, package.value);
        digitalWrite(DRIVE_DIR, LOW);
        stopCount = 0;
    }

    if (package.key == dirs::LYB) {
        Serial.print("LYB: ");
        Serial.println(package.value);
        analogWrite(DRIVE_PWM, package.value);
        digitalWrite(DRIVE_DIR, HIGH);
        stopCount = 0;
    }

    if (package.key == dirs::RXL) {
        Serial.print("RXL: ");
        Serial.println(package.value);
        //digitalWrite(SYSTEM_FUN, HIGH);
        analogWrite(WHEEL_PVM, package.value/WHEEL_DIVIDER);
        digitalWrite(WHEEL_DIR, LOW);
        stopCount = 0;
        stopFunCount = 0;
    }

   if (package.key == dirs::RXR) {
        Serial.print("RXR: ");
        Serial.println(package.value);
        //digitalWrite(SYSTEM_FUN, HIGH);
        analogWrite(WHEEL_PVM, package.value/WHEEL_DIVIDER);
        digitalWrite(WHEEL_DIR, HIGH);
        stopCount = 0;
        stopFunCount = 0;
    }
}

void loop() {
//  digitalWrite(SYSTEM_FUN, LOW);
//
//  
//  //MAIN DRIVE
//  digitalWrite(DRIVE_PWM,LOW);
//  digitalWrite(DRIVE_DIR, LOW);
//
//  digitalWrite(WHEEL_DIR,LOW);
//  digitalWrite(WHEEL_PVM, LOW);
//
//  if (digitalRead(DIGITAL_INPUT_DRIVE_F) == HIGH and digitalRead(DIGITAL_INPUT_DRIVE_B) == LOW) {
//    digitalWrite(DRIVE_PWM, 255/DRIVE_WHEEL_PVM);
//    digitalWrite(DRIVE_DIR, HIGH);
//  }
//
//  if (digitalRead(DIGITAL_INPUT_DRIVE_B) == HIGH and digitalRead(DIGITAL_INPUT_DRIVE_F) == LOW) {
//    digitalWrite(DRIVE_PWM, 255/DRIVE_WHEEL_PVM);
//    digitalWrite(DRIVE_DIR, LOW);
//  }
//
//  //WHEEL DRIVE
//  if (digitalRead(DIGITAL_INPUT_WHEEL_DIR) == HIGH and digitalRead(DIGITAL_INPUT_WHEEL_PVM) == LOW) {
//    digitalWrite(SYSTEM_FUN, HIGH);
//    digitalWrite(WHEEL_DIR, 255/DRIVE_WHEEL_PVM);
//    digitalWrite(WHEEL_PVM, LOW);
//  }
//
//  if (digitalRead(DIGITAL_INPUT_WHEEL_PVM) == HIGH and digitalRead(DIGITAL_INPUT_WHEEL_DIR) == LOW) {
//    digitalWrite(SYSTEM_FUN, HIGH);
//    digitalWrite(WHEEL_PVM, 255/DRIVE_WHEEL_PVM);
//    digitalWrite(WHEEL_DIR, LOW);
//  }
}
