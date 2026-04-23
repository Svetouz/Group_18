#include <Servo.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <VL53L0X.h>
#include "Reach_and_grab.h"


// Servos (PWM pins)
const int SHOULDER_PWM_PIN = 22; //needs 1.8 amps alone
const int ELBOW_PWM_PIN = 24;
const int WRIST_PWM_PIN = 26;
const int GRIPPER_PWM_PIN = 28;

// Motor Driver
const int MOTOR_STEP_PIN = 3;
const int MOTOR_DIR_PIN = 2;

// Motor Driver correspond to MS pins, should be THREE of them,
// of which only TWO are being used!! placeholder pin numbers
// const int MS1 = 1;
// const int MS2= 5;
// const int MS3 = 6;

// Servo variables
Servo shoulder;
Servo elbow;
Servo wrist;
Servo gripper;

// Helper functions

// Stepper - Step/Dir mode 
AccelStepper stepperStepDir(AccelStepper::DRIVER, MOTOR_STEP_PIN, MOTOR_DIR_PIN);


void setup() {
    // Start serial for debugging
    Serial.begin(9600);

    // --- Servo Setup ---
    shoulder.attach(SHOULDER_PWM_PIN);
    elbow.attach(ELBOW_PWM_PIN);
    wrist.attach(WRIST_PWM_PIN);
    gripper.attach(GRIPPER_PWM_PIN);

    stepperStepDir.setMaxSpeed(800);      // steps per second
    stepperStepDir.setAcceleration(500);   // steps per second

}


void loop(){
  shoulder.write(85);
  elbow.write(90);
  wrist.write(50);
  //gripper.write(50);
}
  
  //for finding endopoints of a servo (currently wrist)
  // for (int i = 180; i>0 ; i = i -2)
  // {
  //   writeServos(0,0,0,i);    
  //   Serial.println("Angle: " + i);
  //   delay(250);
  // };