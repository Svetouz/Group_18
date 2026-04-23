#include <Arduino.h>
#include "Servo.h"
#include "Reach_and_grab.h"

//Global Variables

int startShoulder = 84; // 0-84 anticlockwise
int startElbow = 165; // 70-156 clockwise
int startWrist = 0; //  0-180 anticlockwise
int openGrip = 110; // 90-178 anticlockwise
int closeGrip = 177; // 178 pulls more, firmer grip or waste?

// To be reused to store the last written angles, initial values co-
//rrespond to the starting position of the arm
ServoAngles lastAngles = {startShoulder, startElbow, startWrist, closeGrip};


ServoAngles writeServos(int angle1 = -1, int angle2 = -1, int angle3 = -1,
                        int angle4 = -1, int speed = 20){
    //Writes angles to the servos, input in degrees. If an angle is -1,
    //it will not be updated and the last angle will be reused.
    //speed parameter controls smoothness (ms delay between increments)

  if(angle1 != -1) { 
    int currentAngle = lastAngles.angle1;  // Use tracked value, not servo.read()
    if (currentAngle < angle1) {
      for (int angle = currentAngle; angle <= angle1; angle++) {
        shoulder.write(angle);
        delay(speed);
      }
    } else if (currentAngle > angle1) {
      for (int angle = currentAngle; angle >= angle1; angle--) {
        shoulder.write(angle);
        delay(speed);
      }
    }
    lastAngles.angle1 = angle1; 
  }
  delay(100);
  
  if(angle2 != -1) { 
    int currentAngle = lastAngles.angle2;  // Use tracked value, not servo.read()
    if (currentAngle < angle2) {
      for (int angle = currentAngle; angle <= angle2; angle++) {
        elbow.write(angle);
        delay(speed);
      }
    } else if (currentAngle > angle2) {
      for (int angle = currentAngle; angle >= angle2; angle--) {
        elbow.write(angle);
        delay(speed);
      }
    }
    lastAngles.angle2 = angle2; 
  }
  delay(100);
  
  if(angle3 != -1) { 
    int currentAngle = lastAngles.angle3;  // Use tracked value, not servo.read()
    if (currentAngle < angle3) {
      for (int angle = currentAngle; angle <= angle3; angle++) {
        wrist.write(angle);
        delay(speed);
      }
    } else if (currentAngle > angle3) {
      for (int angle = currentAngle; angle >= angle3; angle--) {
        wrist.write(angle);
        delay(speed);
      }
    }
    lastAngles.angle3 = angle3; 
  }
  delay(100);
  
  if(angle4 != -1) { 
    int currentAngle = lastAngles.angle4;  // Use tracked value, not servo.read()
    if (currentAngle < angle4) {
      for (int angle = currentAngle; angle <= angle4; angle++) {
        gripper.write(angle);
        delay(speed);
      }
    } else if (currentAngle > angle4) {
      for (int angle = currentAngle; angle >= angle4; angle--) {
        gripper.write(angle);
        delay(speed);
      }
    }
    lastAngles.angle4 = angle4; 
  }
  delay(100);
  
  return lastAngles;
}

void reachAndGrab(){
  // Opens the gripper and moves the arm into grip position, 
  // close the gripper, and then moves arm into the carrying position.
  //(angles are placeholders and need to be adjusted):
  
  //move above target
  writeServos(30,-1,-1,-1);
  //Open gripper
  writeServos(-1, -1, -1, openGrip);

  // Move into grip position
  writeServos(25, 120, 160, -1);
  
  // Close gripper
  writeServos(-1,-1,-1,closeGrip); 

  // Move back to starting position
  writeServos(startShoulder, startElbow, startWrist, -1);  
}