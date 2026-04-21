#include <Servo.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <VL53L0X.h>
#include "Reach_and_grab.h"


// Servos (PWM pins)
const int SHOULDER_PWM_PIN = 12;
const int ELBOW_PWM_PIN = 11;
const int WRIST_PWM_PIN = 10;
const int GRIPPER_PWM_PIN = 9;

// Motor Driver
const int MOTOR_STEP_PIN = 3;
const int MOTOR_DIR_PIN = 2;
// const int MOTOR_EN_PIN = 7; // is floating

// Motor Driver correspond to MS pins, should be THREE of them,
// of which only TWO are being used!! placeholder pin numbers
const int MOTOR_N1A_PIN = 1;
const int MOTOR_N1B_PIN = 5;
const int MOTOR_N2A_PIN = 6;
const int MOTOR_N2B_PIN = 7;

// ToF Sensor
const int TOF_SDA_PIN = 20;
const int TOF_SCL_PIN = 21;

// Servo variables
Servo shoulder;
Servo elbow;
Servo wrist;
Servo gripper;

// Helper functions

// Stepper - Step/Dir mode (use ONE of these, not both)
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
  writeServos(0);
  //reachAndGrab();
  // //for finding endopoints of a servo (currently wrist)
  // for (int i = 180; i>0 ; i = i -2)
  // {
  //   writeServos(0,0,0,i);    
  //   Serial.println("Angle: " + i);
  //   delay(250);
  // };
}