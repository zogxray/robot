#include <AccelStepper.h>
#include <Servo.h>

// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin 2
#define stepPin 4
#define motorInterfaceType 1
#define servoPin 9

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);
Servo myservo;

int angle = 0;

void setup() 
{
  // Set the maximum speed and acceleration:
  stepper.setSpeed(2000);
  stepper.setAcceleration(3000);
  myservo.attach(servoPin);
  myservo.write(0);
}
 
void loop() {
  // Set the target position:
  stepper.moveTo(1000);
  // Run to target position with set speed and acceleration/deceleration:
  stepper.runToPosition();
 
  delay(1000);
 
  // Move back to zero:
  stepper.moveTo(0);
  stepper.runToPosition();
 
  delay(1000);

    // Tell the servo to go to a particular angle:
  myservo.write(30);
  delay(1000);
    myservo.write(40);
  delay(1000);
  myservo.write(140);
  delay(1000);
  myservo.write(30);
  delay(1000);

}
