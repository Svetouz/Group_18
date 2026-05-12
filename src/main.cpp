#include <Servo.h>
 #include <Keypad.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <VL53L0X.h>
#include "Reach_and_grab.h"


// Servos (PWM pins)
const int SHOULDER_PWM_PIN = 22; //needs 1.8 amps alone
const int ELBOW_PWM_PIN = 24;
const int WRIST_PWM_PIN = 26;
const int GRIPPER_PWM_PIN = 28;

const int stepPin   = 3;  
const int dirPin    = 2;  // Tells the driver which direction to go

const int MAX_SECTORS = 23; // maximum input for keypad
const int STEPS_PER_ROTATION = 500;    
const int STEPS_PER_SECTOR   = (STEPS_PER_ROTATION /(MAX_SECTORS+1)); 

// global variables
Servo shoulder;
Servo elbow;
Servo wrist;
Servo gripper;

// Helper functions
// Stepper - Step/Dir mode 
AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

// --- keypad setup ---
const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {11, 10, 9, 8};
byte colPins[COLS] = {7, 6, 5, 4};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// --- state tracking ---
bool started = false;
int currentSector = 1;    // which sector we are currently entering (1, 2 or 3)
int digitCount = 0;       // which digit of the current sector (1 or 2)
char firstDigit;          // temporary storage for the first digit

// --- sector variables ---
int sector1 = 0;
int sector2 = 0;
int sector3 = 0;

void moveToSector(int sector) {
   long targetStep = sector * STEPS_PER_SECTOR;
  stepper.runToNewPosition(targetStep);
}

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
    // delay(1000);

    stepper.setMaxSpeed(800);      // steps per second
    stepper.setAcceleration(500);   // steps per second
    stepper.setCurrentPosition(0);

}


void loop(){
  //reachAndGrab();
  char key = keypad.getKey();

  if (key) {

    // wait for * to begin
    if (!started) {
      if (key == '*') {
        started = true;
        currentSector = 0;
        digitCount = 0;
        Serial.print("Enter sector 1, digit 1: ");
      }

    } else {

      if (key >= '0' && key <= '9') {

        if (digitCount == 0) {
          // first digit, just store it temporarily
          firstDigit = key;
          digitCount = 1;
          Serial.println(key);
          Serial.print("Sector ");
          Serial.print(currentSector);
          Serial.print(", digit 2: ");

        } else {
          // second digit, combine with first to make the sector number
          int sectorValue = (firstDigit - '0') * 10 + (key - '0');
          Serial.println(key);

          if (sectorValue > MAX_SECTORS || sectorValue < 0) {
            // invalid, ask again
            Serial.print("Sector does not exist! Enter sector ");
            Serial.print(currentSector);
            Serial.print(", digit 1: ");
            digitCount = 0;

          } else {
            // valid, store in the correct variable
            if (currentSector == 1) sector1 = sectorValue;
            if (currentSector == 2) sector2 = sectorValue;
            if (currentSector == 3) sector3 = sectorValue;

            if (currentSector < 3) {
              // move on to next sector
              currentSector++;
              digitCount = 0;
              Serial.print("Enter sector ");
              Serial.print(currentSector);
              Serial.print(", digit 1: ");

            } else {
              // all 3 sectors entered, move the arm
              Serial.println("All sectors received!");
              moveToSector(sector1);
              delay(1000);
              reachAndGrab();
              delay(1000);
              moveToSector(sector2);
              reachAndGrab();
              delay(1000);
              moveToSector(sector3);
              reachAndGrab();       
                     // reset
              started = false;
              currentSector = sector3;
              moveToSector(1);
              delay(2000);
              digitCount = 0;
              Serial.println("Press * to start again...");
            }
          }
        }
      }
    }
  }
}
  