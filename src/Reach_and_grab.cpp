#include <Arduino.h>
#include "Servo.h"
#include "Reach_and_grab.h"

//Global Variables

int startShoulder = 113; // 0-113 anticlockwise
int startElbow = 135; // 0-140 clockwise
int startWrist = 0; //  0-180 anticlockwise
int openGrip = 105; // 100-178 anticlockwise
int closeGrip = 177; // 178 pulls more, firmer grip or waste?

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
  writeServos(80,55,120,openGrip);

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