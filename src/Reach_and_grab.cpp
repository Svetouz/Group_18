#include <Arduino.h>
#include "Servo.h"
#include "Reach_and_grab.h"
#include <Keypad.h>

//Global Variables

int startShoulder = 113; // 0-113 anticlockwise
int startElbow = 135; // 0-140 clockwise
int startWrist = 0; //  0-180 anticlockwise
int openGrip = 105; // 100-178 anticlockwise
int closeGrip = 177; // 178 pulls more, firmer grip or waste?

const int MAX_SECTORS = 18;
bool started = false;
int currentSector = 1;
int digitCount = 0;
char firstDigit;
int sector1 = 0, sector2 = 0, sector3 = 0;

// Mapping: index = input number (0–17), value = logical sector on the original circle (0–23)
const int sectorMap[18] = {
    9,  // input 0 
    18,  // input 1
     5,  // input 2
     8,  // input 3
    21,  // input 4
    16,  // input 5 
     7,  // input 6
    22,  // input 7
    19,  // input 8
     4,  // input 9
    15,  // input 10
     6,  // input 11
    11,  // input 12
    20,  // input 13
    23,  // input 14
    10,  // input 15
    17,  // input 16
     3   // input 17 
};

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

// To be reused to store the last written angles, initial values co-
//rrespond to the starting position of the arm
ServoAngles lastAngles = {startShoulder, startElbow, startWrist, closeGrip};


ServoAngles writeServos(int angle1 = -1, int angle2 = -1, int angle3 = -1,
                        int angle4 = -1, int speed = 15) {
    static unsigned long lastUpdate = 0;
    static int currentAngles[4] = {lastAngles.angle1, lastAngles.angle2, lastAngles.angle3, lastAngles.angle4};
    static int targetAngles[4] = {lastAngles.angle1, lastAngles.angle2, lastAngles.angle3, lastAngles.angle4};

    // Update target angles if new values are provided
    if (angle1 != -1) targetAngles[0] = angle1;
    if (angle2 != -1) targetAngles[1] = angle2;
    if (angle3 != -1) targetAngles[2] = angle3;
    if (angle4 != -1) targetAngles[3] = angle4;
    
    while(currentAngles[0] != targetAngles[0] || currentAngles[1] != targetAngles[1] || 
          currentAngles[2] != targetAngles[2] || currentAngles[3] != targetAngles[3]) {
        // Check if it's time to update the servo positions
        if (millis() - lastUpdate >= speed) {
            lastUpdate = millis();

            // Update each servo incrementally
            for (int i = 0; i < 4; i++) {
                if (currentAngles[i] < targetAngles[i]) {
                    currentAngles[i]++;
                } else if (currentAngles[i] > targetAngles[i]) {
                    currentAngles[i]--;
                }
            }

            // Write the updated angles to the servos
            shoulder.write(currentAngles[0]);
            elbow.write(currentAngles[1]);
            wrist.write(currentAngles[2]);
            gripper.write(currentAngles[3]);

            // Update the lastAngles structure
            lastAngles = {currentAngles[0], currentAngles[1], currentAngles[2], currentAngles[3]};
        }
    }

    return lastAngles;
}

void reachAndGrab(){
  // Opens the gripper and moves the arm into grip position, 
  // close the gripper, and then moves arm into the carrying position.
  //(angles are placeholders and need to be adjusted):

  // Move above target
  writeServos(80,55,123,openGrip);

  // Move into grip position
  writeServos(40,-1,-1,-1);

  // Close gripper
  writeServos(-1,-1,-1,closeGrip);
  delay(500);

  // Move to carry position
  writeServos(60,55,-1,-1);

}

void setDown(){
  // Moves the arm into grip position, opens the gripper at the bottom,
  // closes the gripper, and then moves arm back to resting position above target.
  
  // Move into grip position
  writeServos(43,55,120,-1);

  // Open gripper at the bottom
  writeServos(-1,-1,-1,120);
  delay(500);

  //lift gripper
  writeServos(60,80,-1,-1);

  // Close gripper
  writeServos(-1,-1,-1,closeGrip);
  delay(500);

  // Move back to resting position above target
  writeServos(80,75,120,-1);
}

InputResult getInputs(){
  char key = keypad.getKey();
  int inputsTaken = 0;

    if (key) {
        if (!started) {
            if (key == '*') {
                started = true;
                currentSector = 1;//
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

                    if (sectorValue > MAX_SECTORS || sectorValue < 0 ) {
                        Serial.print("Invalid sector! Enter sector ");
                        Serial.print(currentSector);
                        Serial.print(", digit 1: ");
                        digitCount = 0;
                    } else {
                        // valid, store in the correct variable
                        if (currentSector == 1) sector1 = sectorMap[sectorValue];
                        if (currentSector == 2) sector2 = sectorMap[sectorValue];
                        if (currentSector == 3) sector3 = sectorMap[sectorValue];

                        if (currentSector < 3) {
                            currentSector++;
                            digitCount = 0;
                            Serial.print("Enter sector ");
                            Serial.print(currentSector);
                            Serial.print(", digit 1: ");
                        } else {
                            inputsTaken = 1;
                            started = false;
                            digitCount = 0;
                        }
                    }
                }
            }
        }
    }
    return {sector1, sector2, sector3, inputsTaken};
}