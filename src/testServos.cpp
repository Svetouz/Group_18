#include <Servo.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <VL53L0X.h>
#include "Reach_and_grab.h"


// Servos (PWM pins)
const int SHOULDER_PWM_PIN = 22; //needs 1.8 amps alone
const int ELBOW_PWM_PIN = 24;
const int WRIST_PWM_PIN = 26;
const int GRIPPER_PWM_PIN = 28;

// Servo variables
Servo shoulder;
Servo elbow;
Servo wrist;
Servo gripper;

// Helper functions


void setup() {
    // Start serial for debugging
    Serial.begin(9600);
    delay(1000); // Wait for serial to stabilize
    
    Serial.println("Enter servo name (shoulder, elbow, wrist, gripper):");

    //Servos Setup - attach and immediately write to establish position
    shoulder.attach(SHOULDER_PWM_PIN);
    shoulder.write(84);
    
    elbow.attach(ELBOW_PWM_PIN);
    elbow.write(165);
    
    wrist.attach(WRIST_PWM_PIN);
    wrist.write(0);
    
    gripper.attach(GRIPPER_PWM_PIN);
    gripper.write(177);
    
    // Sync lastAngles with initial positions so writeServos knows where we are
    lastAngles = {84, 165, 0, 177};
    
    delay(1000);

}

// //writeServos test
// void loop(){
//   writeServos(84, 165, 0, 177, 30);

// }

// //writeServos test with serial input
// void loop(){
//   if (Serial.available() > 0) {
//     Serial.println("Enter shoulder angle (-1 to skip):");
//     while (Serial.available() == 0) {
//       delay(10);
//     }
//     int shoulder_angle = Serial.readStringUntil('\n').toInt();
    
//     Serial.println("Enter elbow angle (-1 to skip):");
//     while (Serial.available() == 0) {
//       delay(10);
//     }
//     int elbow_angle = Serial.readStringUntil('\n').toInt();
    
//     Serial.println("Enter wrist angle (-1 to skip):");
//     while (Serial.available() == 0) {
//       delay(10);
//     }
//     int wrist_angle = Serial.readStringUntil('\n').toInt();
    
//     Serial.println("Enter gripper angle (-1 to skip):");
//     while (Serial.available() == 0) {
//       delay(10);
//     }
//     int gripper_angle = Serial.readStringUntil('\n').toInt();
    
//     // Call writeServos with all angles
//     writeServos(shoulder_angle, elbow_angle, wrist_angle, gripper_angle, 20);
//     Serial.println("Servos moved to (" + String(shoulder_angle) + ", " + String(elbow_angle) + ", " + String(wrist_angle) + ", " + String(gripper_angle) + ")\n");
//   }
// }

//Old serial input testing (without writeServos)
void loop(){
  if (Serial.available() > 0) {
    String servo_name = Serial.readStringUntil('\n');
    servo_name.trim();
    servo_name.toLowerCase();
    
    Serial.println("Enter angle (0-180):");
    while (Serial.available() == 0) {
      delay(10);
    }
    
    String angle_str = Serial.readStringUntil('\n');
    angle_str.trim();
    int angle = angle_str.toInt();
    
    // Constrain angle to valid range
    angle = constrain(angle, 0, 180);
    
    // Write to appropriate servo
    if (servo_name == "shoulder") {
      shoulder.write(angle);
      Serial.println("Shoulder set to " + String(angle) + " degrees");
    } else if (servo_name == "elbow") {
      elbow.write(angle);
      Serial.println("Elbow set to " + String(angle) + " degrees");
    } else if (servo_name == "wrist") {
      wrist.write(angle);
      Serial.println("Wrist set to " + String(angle) + " degrees");
    } else if (servo_name == "gripper") {
      gripper.write(angle);
      Serial.println("Gripper set to " + String(angle) + " degrees");
    } else {
      Serial.println("Unknown servo: " + servo_name);
    }
    
    Serial.println("\nEnter servo name (shoulder, elbow, wrist, gripper):");
  }
}
  
  // for finding endopoints of a servo (currently wrist)
  // for (int i = 180; i>0 ; i = i -2)
  // {
  //   writeServos(0,0,0,i);    
  //   Serial.println("Angle: " + i);
  //   delay(250);
  // };