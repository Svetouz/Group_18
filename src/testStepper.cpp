#include <AccelStepper.h>

// Define Stepper Motor Pins
const int DIR_PIN = 2;
const int STEP_PIN = 3;

// Define Microstepping Jumper Pins
const int MS1_PIN = 34;
const int MS2_PIN = 36;
const int MS3_PIN = 38;

const unsigned long RUN_TIME_MS = 1500; 
const int STEP_DELAY_US = 112; // Adjusted for 1/16 microstepping

AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
    Serial.begin(9600);

    // Configure Microstepping pins
    pinMode(MS1_PIN, OUTPUT);
    pinMode(MS2_PIN, OUTPUT);
    pinMode(MS3_PIN, OUTPUT);

    digitalWrite(MS1_PIN, HIGH);
    digitalWrite(MS2_PIN, HIGH);
    digitalWrite(MS3_PIN, HIGH);

    Serial.println("Microstepping initialized to 1/16 step mode.");

    // Configure AccelStepper
    stepper.setMaxSpeed(4000); // Default max speed
    stepper.setAcceleration(2000); // Default acceleration

    Serial.println("Default max speed: 4000 steps/s");
    Serial.println("Default acceleration: 2000 steps/s^2");
    Serial.println("Enter 'maxspeed <value>' or 'accel <value>' to adjust settings.");
}

void loop() {
    // Wait for Serial input before starting the iteration
    Serial.println("Enter 'start' to begin the iteration or adjust settings with 'maxspeed <value>' or 'accel <value>'.");
    while (!Serial.available()) {
        // Wait for user input
    }

    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.startsWith("maxspeed")) {
        int newMaxSpeed = input.substring(9).toInt();
        if (newMaxSpeed > 0) {
            stepper.setMaxSpeed(newMaxSpeed);
            Serial.println("Max speed updated to: " + String(newMaxSpeed) + " steps/s");
        } else {
            Serial.println("Invalid max speed value.");
        }
    } else if (input.startsWith("accel")) {
        int newAccel = input.substring(6).toInt();
        if (newAccel > 0) {
            stepper.setAcceleration(newAccel);
            Serial.println("Acceleration updated to: " + String(newAccel) + " steps/s^2");
        } else {
            Serial.println("Invalid acceleration value.");
        }
    } else if (input == "start") {
        // Perform one iteration: 0 -> 360 -> 0
        stepper.moveTo(16*500); // Move 4000 steps forward (360 degrees)
        while (stepper.distanceToGo() != 0) {
            stepper.run();
        }

        delay(300); // Pause before reversing

        stepper.moveTo(0); // Move back to the starting position
        while (stepper.distanceToGo() != 0) {
            stepper.run();
        }

        delay(1000); // Pause before asking for new input
        Serial.println("Iteration complete.");
    } else {
        Serial.println("Invalid command. Use 'start', 'maxspeed <value>', or 'accel <value>'.");
    }
}