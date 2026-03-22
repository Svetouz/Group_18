#include <Servo.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <VL53L0X.h>

// ============================================
// PIN DEFINITIONS
// ============================================

// === Servos (PWM pins) ===
const int SERVO1_PWM_PIN = 2;
const int SERVO2_PWM_PIN = 3;
const int SERVO3_PWM_PIN = 4;

// === Motor Driver - Step/Dir Mode ===
const int MOTOR_STEP_PIN = 5;
const int MOTOR_DIR_PIN = 6;
const int MOTOR_EN_PIN = 7;

// === Motor Driver - Direct Coil Control (alternative) ===
const int MOTOR_N1A_PIN = 8;
const int MOTOR_N1B_PIN = 9;
const int MOTOR_N2A_PIN = 10;
const int MOTOR_N2B_PIN = 11;

// === ToF Sensor (I2C - fixed pins on Mega) ===
const int TOF_SDA_PIN = 20;
const int TOF_SCL_PIN = 21;

// === Ultrasonic Sensor ===
const int ULTRASONIC_TRIG_PIN = 22;
const int ULTRASONIC_ECHO_PIN = 23;

// ============================================
// OBJECT DECLARATIONS
// ============================================

// Servos
Servo servo1;
Servo servo2;
Servo servo3;

// Stepper - Step/Dir mode (use ONE of these, not both)
AccelStepper stepperStepDir(AccelStepper::DRIVER, MOTOR_STEP_PIN, MOTOR_DIR_PIN);

// Stepper - Direct coil control (alternative)
// AccelStepper stepperCoil(AccelStepper::FULL4WIRE, MOTOR_N1A_PIN, MOTOR_N1B_PIN, MOTOR_N2A_PIN, MOTOR_N2B_PIN);

// ToF Sensor
VL53L0X tofSensor;

// ============================================
// SETUP
// ============================================

void setup() {
    // Start serial for debugging
    Serial.begin(9600);
    Serial.println("SIM2 Pick & Place Starting...");

    // --- Servo Setup ---
    servo1.attach(SERVO1_PWM_PIN);
    servo2.attach(SERVO2_PWM_PIN);
    servo3.attach(SERVO3_PWM_PIN);

    // Move servos to starting position (90 degrees = center)
    servo1.write(90);
    servo2.write(90);
    servo3.write(90);

    // --- Stepper Setup (Step/Dir mode) ---
    pinMode(MOTOR_EN_PIN, OUTPUT);
    digitalWrite(MOTOR_EN_PIN, LOW);  // LOW = enabled on most drivers

    stepperStepDir.setMaxSpeed(1000);      // steps per second
    stepperStepDir.setAcceleration(500);   // steps per second per second

    // --- ToF Sensor Setup (I2C) ---
    Wire.begin();  // Uses pins 20 (SDA) and 21 (SCL) automatically on Mega

    if (!tofSensor.init()) {
        Serial.println("ToF sensor not found!");
    } else {
        Serial.println("ToF sensor initialized");
        tofSensor.setTimeout(500);
        tofSensor.startContinuous();
    }

    // --- Ultrasonic Sensor Setup ---
    pinMode(ULTRASONIC_TRIG_PIN, OUTPUT);
    pinMode(ULTRASONIC_ECHO_PIN, INPUT);

    Serial.println("Setup complete");
}