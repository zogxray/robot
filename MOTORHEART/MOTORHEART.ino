#include <Wire.h> 
#include <Servo.h>

//#define DEBUG

const int FORWARD_LIGHT_HB = 3;
const int LEFT_LIGHT_HB = 5;
const int RIGHT_LIGHT_HB = 6;
const int BACK_LIGHT_HB = 11;

//MAIN DRIVE
const int DRIVE_PWM = 9;
//WHEEL DRIVE
const int WHEEL_PVM = 10;

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
  
enum slaves {HEART};

typedef struct {
  byte key;
  byte value;
} Package;

Package package;

int ledState = 0;
int prevLedState = 0;

int backLightPower = 2;

const int CENTER_POINT_OF_MOTOR = 1500;
const int MIN_POINT_OF_MOTOR = CENTER_POINT_OF_MOTOR-(CENTER_POINT_OF_MOTOR/2);
const int MAX_POINT_OF_MOTOR = CENTER_POINT_OF_MOTOR+(CENTER_POINT_OF_MOTOR/2);
int motorPowerCorrection = CENTER_POINT_OF_MOTOR/2;
int motorPowerCorrectionStep = 50;

int trimmedWheelAngle = 1500;
int lastWheelAngle = 1500;
int wheelAngle = 1500;

int pulseLedInterval = 250;
unsigned long previousMillisPulse;

Servo motor, wheel;

void setup() { 
  #ifdef DEBUG
  Serial.begin(115200);
  Serial.println("I am heart");
  #endif

  pinMode(LEFT_LIGHT_HB, OUTPUT);
  pinMode(RIGHT_LIGHT_HB, OUTPUT);

  pinMode(FORWARD_LIGHT_HB, OUTPUT);
  pinMode(BACK_LIGHT_HB, OUTPUT);

  Wire.begin(slaves::HEART); 
  Wire.onReceive(receiveEvent);

  motor.attach(DRIVE_PWM, MIN_POINT_OF_MOTOR, MAX_POINT_OF_MOTOR);  // attaches the servo on pin 9 to the servo object
  wheel.attach(WHEEL_PVM, 1000, 2000);
  
  Serial.println("I am start");
}

void receiveEvent(int bytes) {
    Wire.readBytes((byte*)&package, sizeof(package));

    if (package.key == buttons::LIGHT) {
      ledState += 64;
      if (ledState > 256) {
        ledState = 0;
      }
      
      #ifdef DEBUG
        Serial.print(F("LIGHT: "));
        Serial.println(ledState);
      #endif
    }

    if (package.key == buttons::TRIM_WHEEL_LEFT) {
      #ifdef DEBUG
        Serial.print(F("TRIM WHEEL LEFT: "));
        Serial.println(ledState);
      #endif

      trimmedWheelAngle -= 5;
      analogWrite(LEFT_LIGHT_HB, 8); 
    }

    
    if (package.key == buttons::TRIM_WHEEL_RIGHT) {
      #ifdef DEBUG
        Serial.print(F("TRIM WHEEL RIGHT: "));
        Serial.println(ledState);
      #endif

      trimmedWheelAngle += 5;
      analogWrite(RIGHT_LIGHT_HB, 8);
    }

    if (package.key == buttons::POWER_DOWN) {
      #ifdef DEBUG
        Serial.print(F("POWER DOWN: "));
        Serial.println(ledState);
      #endif

      motorPowerCorrection +=motorPowerCorrectionStep;

      if (motorPowerCorrection > CENTER_POINT_OF_MOTOR/2) {
        motorPowerCorrection = CENTER_POINT_OF_MOTOR/2;
      } else {
        analogWrite(LEFT_LIGHT_HB, 8);
      } 
    }

    if (package.key == buttons::POWER_UP) {
      #ifdef DEBUG
        Serial.print(F("POWER UP: "));
        Serial.println(ledState);
      #endif

      motorPowerCorrection -= motorPowerCorrectionStep;

      if (motorPowerCorrection < 0) {
        motorPowerCorrection = 0;
      } else {
         analogWrite(RIGHT_LIGHT_HB, 8);
      }
    }
      
    if (package.key == buttons::LYF) {
        #ifdef DEBUG
          Serial.print("LYF: ");
          Serial.println(package.value);
        #endif
        int val = map(package.value, 0, 255, CENTER_POINT_OF_MOTOR, MAX_POINT_OF_MOTOR-motorPowerCorrection);
        #ifdef DEBUG
          Serial.print(" === ");
          Serial.println(val);
        #endif
        motor.writeMicroseconds(val);
    }

    if (package.key == buttons::LYB) {
        #ifdef DEBUG
          Serial.print("LYB: ");
          Serial.print(package.value);
        #endif
        int val = map(package.value, 0, 255, CENTER_POINT_OF_MOTOR-10, CENTER_POINT_OF_MOTOR-100);
        #ifdef DEBUG
          Serial.print(" === ");
          Serial.println(val);
        #endif
        motor.writeMicroseconds(val);
        backLightPower = 32;
    }

    if (package.key == buttons::RXL) {
        #ifdef DEBUG
          Serial.print("RXL: ");
          Serial.println(package.value);
        #endif

        wheelAngle = map(package.value, 0, 255, trimmedWheelAngle, trimmedWheelAngle-300);
        
        #ifdef DEBUG
          Serial.print("RXR ANGLE: ");
          Serial.println(wheelAngle);
        #endif
        
        analogWrite(LEFT_LIGHT_HB, 8);  
    }

   if (package.key == buttons::RXR) {
        #ifdef DEBUG
          Serial.print("RXR: ");
          Serial.println(package.value);
        #endif
        wheelAngle = map(package.value, 0, 255, trimmedWheelAngle, trimmedWheelAngle+300);
        
        #ifdef DEBUG
          Serial.print("RXR ANGLE: ");
          Serial.println(wheelAngle);
        #endif
        
        analogWrite(RIGHT_LIGHT_HB, 8);
    }
}

void loop() {
    if (millis() - previousMillisPulse >= pulseLedInterval) {
      previousMillisPulse = millis();
      #ifdef DEBUG
        Serial.print(F("Timer tick: "));
      #endif

      wheelAngle = trimmedWheelAngle;
      motor.writeMicroseconds(CENTER_POINT_OF_MOTOR);

      analogWrite(LEFT_LIGHT_HB, 0);
      analogWrite(RIGHT_LIGHT_HB, 0);

      backLightPower = 2;
    }
  
   if (lastWheelAngle != wheelAngle) {
       wheel.writeMicroseconds(wheelAngle);
       lastWheelAngle = wheelAngle;
   }

   if (ledState != prevLedState) {
      analogWrite(FORWARD_LIGHT_HB, ledState);
      #ifdef DEBUG
        Serial.print(F("Change led state: "));
        Serial.println(ledState);
      #endif
      prevLedState = ledState;
   }

   analogWrite(BACK_LIGHT_HB, backLightPower);
}
