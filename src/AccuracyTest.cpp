#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::DRIVER, 3, 2); // STEP=3, DIR=2

void setup() {
  Serial.begin(9600);
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
}

void loop() {
  // Move forward 200 steps
  stepper.runToNewPosition(200);
  delay(500);

  // Move back to start
  stepper.runToNewPosition(0);
  delay(500);

  // Print position — should always be 0
  Serial.print("Position: ");
  Serial.println(stepper.currentPosition());
}
