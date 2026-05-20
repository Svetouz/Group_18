#include <Servo.h>
#include <AccelStepper.h>
#include <Keypad.h>
#include "Reach_and_grab.h"

// Servo Pins
const int SHOULDER_PWM_PIN = 10;
const int ELBOW_PWM_PIN = 11;
const int WRIST_PWM_PIN = 12;
const int GRIPPER_PWM_PIN = 13;

// Stepper Pins
const int DIR_PIN = 2;
const int STEP_PIN = 3;

// Microstepping Pins
const int MS1_PIN = 34;
const int MS2_PIN = 36;
const int MS3_PIN = 38;

// Stepper Constants
const int MAX_SECTORS = 23;
const int STEPS_PER_ROTATION = (16 * 500) + 333;
const int STEPS_PER_SECTOR = STEPS_PER_ROTATION / (MAX_SECTORS + 1);
const int HOME_OFFSET = 0.5 * STEPS_PER_SECTOR; // Adjust if needed

// Servo Variables
Servo shoulder;
Servo elbow;
Servo wrist;
Servo gripper;

// Stepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// Keypad Setup
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {22, 24, 26, 28};
byte colPins[COLS] = {23, 25, 27, 29};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// State Tracking
bool started = false;
int currentSector = 1;
int digitCount = 0;
char firstDigit;
int sector1 = 0, sector2 = 0, sector3 = 0;

void moveToSector(int sector) {
    long targetStep = sector * STEPS_PER_SECTOR + HOME_OFFSET;
    stepper.moveTo(targetStep);
    while (stepper.distanceToGo() != 0) {
        stepper.run();
    }
}

void setup() {
    Serial.begin(9600);
    delay(1000);

    // Servo Setup
    shoulder.attach(SHOULDER_PWM_PIN);
    elbow.attach(ELBOW_PWM_PIN);
    wrist.attach(WRIST_PWM_PIN);
    gripper.attach(GRIPPER_PWM_PIN);

    shoulder.write(130);
    elbow.write(130);
    wrist.write(0);
    gripper.write(177);

    delay(300);

    // Stepper Setup
    pinMode(MS1_PIN, OUTPUT);
    pinMode(MS2_PIN, OUTPUT);
    pinMode(MS3_PIN, OUTPUT);

    digitalWrite(MS1_PIN, HIGH);
    digitalWrite(MS2_PIN, HIGH);
    digitalWrite(MS3_PIN, HIGH);

    stepper.setMaxSpeed(4000);
    stepper.setAcceleration(2000);
    stepper.setCurrentPosition(STEPS_PER_SECTOR - STEPS_PER_SECTOR * 0.5);

    Serial.println("Press * to start entering sectors.");
}

void loop() {
    char key = keypad.getKey();

    if (key) {
        if (!started) {
            if (key == '*') {
                started = true;
                currentSector = 1;
                digitCount = 0;
                Serial.print("Enter sector 1, digit 1: ");
            }
        } else {
            if (key >= '0' && key <= '9') {
                if (digitCount == 0) {
                    firstDigit = key;
                    digitCount = 1;
                    Serial.println(key);
                    Serial.print("Sector ");
                    Serial.print(currentSector);
                    Serial.print(", digit 2: ");
                } else {
                    int sectorValue = (firstDigit - '0') * 10 + (key - '0');
                    Serial.println(key);

                    if (sectorValue > MAX_SECTORS || sectorValue < 0) {
                        Serial.print("Invalid sector! Enter sector ");
                        Serial.print(currentSector);
                        Serial.print(", digit 1: ");
                        digitCount = 0;
                    } else {
                        if (currentSector == 1) sector1 = sectorValue;
                        if (currentSector == 2) sector2 = sectorValue;
                        if (currentSector == 3) sector3 = sectorValue;

                        if (currentSector < 3) {
                            currentSector++;
                            digitCount = 0;
                            Serial.print("Enter sector ");
                            Serial.print(currentSector);
                            Serial.print(", digit 1: ");
                        } else {
                            Serial.println("All sectors received!");

                            // Execute sequence
                            moveToSector(sector1);
                            reachAndGrab();
                            delay(1000);

                            moveToSector(sector2);
                            setDown();
                            delay(1000);
                            reachAndGrab();
                            delay(1000);

                            moveToSector(sector3);
                            setDown();
                            delay(1000);

                            moveToSector(0);

                            // Reset
                            started = false;
                            digitCount = 0;
                            Serial.println("Press * to start again...");
                        }
                    }
                }
            }
        }
    }
}