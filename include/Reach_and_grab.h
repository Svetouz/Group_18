#ifndef REACH_AND_GRAB_H
#define REACH_AND_GRAB_H

#include <Servo.h>

struct ServoAngles{
    int angle1;
    int angle2;
    int angle3;
    int angle4;
};

extern Servo shoulder;
extern Servo elbow;
extern Servo wrist;
extern Servo gripper;
extern ServoAngles lastAngles;

ServoAngles writeServos(int angle1 = -1, int angle2 = -1, int angle3 = -1, int angle4 = -1);
void reachAndGrab();

#endif