#include <Arduino.h>
#include "Servo.h"
#include "Reach_and_grab.h"

//Global Variables

int openGrip = 0; //Angle that opens the gripper, placeholder value, 
int closeGrip = 30; //Angle that closes the gripper, placeholder value
int startShoulder = 90; //Starting angle for shoulder, placeholder value
int startElbow = 90; //Starting angle for elbow, placeholder value
int startWrist = 90; //Starting angle for wrist, placeholder value

// To be reused to store the last written angles, initial values co-
//rrespond to the starting position of the arm
ServoAngles lastAngles = {startShoulder, startElbow, startWrist, closeGrip}; 

ServoAngles writeServos(int angle1 = -1, int angle2 = -1, int angle3 = -1,
                        int angle4 = -1){
    //Writes angles to the servos, input in degrees. If an angle is -1,
    //it will not be updated and the last angle will be reused.

  if(angle1 != -1) { shoulder.write(angle1); lastAngles.angle1 = angle1; }
  delay(250);
  if(angle2 != -1) { elbow.write(angle2); lastAngles.angle2 = angle2; }
  delay(250);
  if(angle3 != -1) { wrist.write(angle3); lastAngles.angle3 = angle3; }
  delay(250);
  if(angle4 != -1) { gripper.write(angle4); lastAngles.angle4 = angle4; }
  delay(250);
  
  return lastAngles;
}

void reachAndGrab(){
  // Opens the gripper and moves the arm into grip position, 
  // close the gripper, and then moves arm into the carrying position.
  //(angles are placeholders and need to be adjusted):
  
  //Open gripper
  ServoAngles openGripper = writeServos(-1, -1, -1,openGrip);

  // Move into grip position
  ServoAngles intoGripPosition = writeServos(45, 90, 135, -1);
  
  // Close gripper
  ServoAngles closeGripper = writeServos(45, 90, 135, closeGrip); 

  // Move back to starting position
  ServoAngles backToStart = writeServos(90, 90, 90, -1);  
}