#include <Servo.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <VL53L0X.h>
#include "Reach_and_grab.h"


// Servos (PWM pins)
const int SHOULDER_PWM_PIN = 8; //needs 1.8 amps alone
const int ELBOW_PWM_PIN = 9;
const int WRIST_PWM_PIN = 10;
const int GRIPPER_PWM_PIN = 11;

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
    shoulder.write(113);
    
    elbow.attach(ELBOW_PWM_PIN);
    elbow.write(90);
    
    wrist.attach(WRIST_PWM_PIN);
    wrist.write(0);
    
    gripper.attach(GRIPPER_PWM_PIN);
    gripper.write(177);
    
    // Sync lastAngles with initial positions so writeServos knows where we are
    lastAngles = {113, 90, 0, 177};
    
    delay(1000);

}


//Old serial input testing (without writeServos)
void loop(){
  static int targetAngles[4] = {113, 90, 0, 177}; // Default angles
  static String currentServo = ""; // Remember the last servo name

  // Check for Serial input
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    input.toLowerCase();

    // Check if the input is a servo name
    if (input == "shoulder" || input == "elbow" || input == "wrist" || input == "gripper") {
      currentServo = input;
      Serial.println("Selected servo: " + currentServo);
      Serial.println("Enter angle:");
      return;
    }

    // If the input is not a servo name, treat it as an angle
    if (currentServo != "") {
      // Check if the input is a valid integer
      bool isValidInt = true;
      for (unsigned int i = 0; i < input.length(); i++) {
        if (!isDigit(input[i])) {
          isValidInt = false;
          break;
        }
      }

      if (!isValidInt) {
        Serial.println("Invalid input. Please enter a valid integer angle.");
        Serial.println("Enter angle:");
        return;
      }

      int angle = input.toInt();

      // Constrain angle based on the current servo
      if (currentServo == "shoulder") {
        if (angle < 0 || angle > 113) {
          Serial.println("Error: Shoulder angle must be between 0 and 113.");
        } else {
          targetAngles[0] = angle;
          Serial.println("Shoulder set to " + String(angle) + " degrees");
        }
      } else if (currentServo == "elbow") {
        if (angle < 0 || angle > 130) {
          Serial.println("Error: Elbow angle must be between 0 and 130.");
        } else {
          targetAngles[1] = angle;
          Serial.println("Elbow set to " + String(angle) + " degrees");
        }
      } else if (currentServo == "wrist") {
        if (angle < 0 || angle > 180) {
          Serial.println("Error: Wrist angle must be between 0 and 180.");
        } else {
          targetAngles[2] = angle;
          Serial.println("Wrist set to " + String(angle) + " degrees");
        }
      } else if (currentServo == "gripper") {
        if (angle < 90 || angle > 177) {
          Serial.println("Error: Gripper angle must be between 90 and 177.");
        } else {
          targetAngles[3] = angle;
          Serial.println("Gripper set to " + String(angle) + " degrees");
        }
      }
    } else {
      Serial.println("Please specify a servo name first (shoulder, elbow, wrist, gripper).");
    }

    Serial.println("\nEnter servo name (shoulder, elbow, wrist, gripper) or angle:");
  }

  // Continuously call writeServos to update servo positions
  writeServos(targetAngles[0], targetAngles[1], targetAngles[2], targetAngles[3], 20);
}