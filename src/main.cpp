#include <Servo.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <VL53L0X.h>


// Servos (PWM pins)
const int SHOULDER_PWM_PIN = 2;
const int ELBOW_PWM_PIN = 3;
const int WRIST_PWM_PIN = 4;
const int GRIPPER_PWM_PIN = 12;

// Motor Driver
const int MOTOR_STEP_PIN = 5;
const int MOTOR_DIR_PIN = 6;
const int MOTOR_EN_PIN = 7;

// Motor Driver
const int MOTOR_N1A_PIN = 8;
const int MOTOR_N1B_PIN = 9;
const int MOTOR_N2A_PIN = 10;
const int MOTOR_N2B_PIN = 11;

// ToF Sensor
const int TOF_SDA_PIN = 20;
const int TOF_SCL_PIN = 21;

// Servo variables
Servo shoulder;
Servo elbow;
Servo wrist;
Servo gripper;

// Helper functions
void writeServos(int angle1,int angle2, int angle3, int angle4){
  //Writes angles to the servos, input in degrees.
  shoulder.write(angle1);
  elbow.write(angle2);
  wrist.write(angle3);
  gripper.write(angle4);
}

// Stepper - Step/Dir mode (use ONE of these, not both)
AccelStepper stepperStepDir(AccelStepper::DRIVER, MOTOR_STEP_PIN, MOTOR_DIR_PIN);

// Stepper - Direct coil control (alternative)
// AccelStepper stepperCoil(AccelStepper::FULL4WIRE, MOTOR_N1A_PIN, MOTOR_N1B_PIN, MOTOR_N2A_PIN, MOTOR_N2B_PIN);

// ToF Sensor
VL53L0X tofSensor;


void setup() {
    // Start serial for debugging
    Serial.begin(9600);

    // --- Servo Setup ---
    shoulder.attach(SHOULDER_PWM_PIN);
    elbow.attach(ELBOW_PWM_PIN);
    wrist.attach(WRIST_PWM_PIN);
    gripper.attach(GRIPPER_PWM_PIN);

    // Move servos to starting position 
    writeServos(90,90,90,90);
    // shoulder.write(90);
    // elbow.write(90);
    // wrist.write(90);
    // gripper.write(90);

    // Stepper Setup (Step/Dir mode)
    pinMode(MOTOR_EN_PIN, OUTPUT);
    digitalWrite(MOTOR_EN_PIN, LOW);  // LOW = enabled on most drivers

    stepperStepDir.setMaxSpeed(1000);      // steps per second
    stepperStepDir.setAcceleration(500);   // steps per second

    // // ToF Sensor Setup (I2C)
    // Wire.begin();  // Uses pins 20 (SDA) and 21 (SCL) automatically on Mega

    // if (!tofSensor.init()) {
    //     Serial.println("ToF sensor not found!");
    // } else {
    //     Serial.println("ToF sensor initialized");
    //     tofSensor.setTimeout(500);
    //     tofSensor.startContinuous();
    // }

}



void loop(){

}