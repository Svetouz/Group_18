#include <Servo.h>
#include <AccelStepper.h>
#include "Reach_and_grab.h"
#include <QTRSensors.h> 

// Servo Pins
const int SHOULDER_PWM_PIN = 10;
const int ELBOW_PWM_PIN = 11;
const int WRIST_PWM_PIN = 12;
const int GRIPPER_PWM_PIN = 13;

// Stepper Pins
const int DIR_PIN = 8;
const int STEP_PIN = 9;

// Microstepping Pins
const int MS1_PIN = 34;
const int MS2_PIN = 36;
const int MS3_PIN = 38;

// Stepper Constants
const int MAX_SECTORS = 23;
const int STEPS_PER_ROTATION = (16 * 500) + 333;
const int STEPS_PER_SECTOR = STEPS_PER_ROTATION / (MAX_SECTORS + 1);
const int HOME_OFFSET = 0.5 * STEPS_PER_SECTOR; // Adjust if needed

// Motor pins
const int ENA = 2;
const int IN1 = 3;
const int IN2 = 4;
const int ENB = 7;
const int IN3 = 5;
const int IN4 = 6;

// Motor Speeds 
const int CRUISE_PWM  = 65; 
const int TURN_PWM    = 120; 
const int REVERSE_PWM = -40; 

// Servo Variables
Servo shoulder;
Servo elbow;
Servo wrist;
Servo gripper;

// Stepper
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

// QTR-8 Configuration
QTRSensors qtr;
const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

// flags for movements
int lastDirection = 0; 
bool primedForStop = false; 

void moveToSector(int sector) {
    long targetStep = sector * STEPS_PER_SECTOR + HOME_OFFSET;
    stepper.moveTo(targetStep);
    while (stepper.distanceToGo() != 0) {
        stepper.run();
    }
}

void setMotorRaw(int leftPWM, int rightPWM) {
  if (rightPWM >= 0) {
    digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH); 
    analogWrite(ENA, rightPWM);
  } else {
    digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
    analogWrite(ENA, -rightPWM);
  }

  if (leftPWM >= 0) {
    digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    analogWrite(ENB, leftPWM);
  } else {
    digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    analogWrite(ENB, -leftPWM);
  }
}

void setup() {
    Serial.begin(9600);
    delay(1000);
    pinMode(ENA, OUTPUT); pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

    qtr.setTypeRC(); 
    qtr.setSensorPins((const uint8_t[]){52, 50, 48, 46, 53, 51, 49, 47}, SensorCount);


    Serial.println("Starting calibration...");
    // Calibration sequence 
    for (uint16_t i = 0; i < 200; i++) {
      qtr.calibrate();
      qtr.read(sensorValues);
      
      Serial.print("Calib ");
      Serial.print(i);
      Serial.print(": ");
      for (uint8_t j = 0; j < SensorCount; j++) {
        Serial.print(sensorValues[j]);
        if (j < SensorCount - 1) {
          Serial.print(", ");
        }
      }
      Serial.println();
      delay(20);
    }
    Serial.println("Calibration complete.");

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
  static InputResult inputs = {0, 0, 0, false};
  static bool inputsCollected = false;
  
  // Only collect inputs once
  if (!inputsCollected) {
    inputs = getInputs();
    if (!inputs.inputsTaken) {
      return;  // Still waiting for keypad input
    }
    inputsCollected = true;
    Serial.println("=== Inputs taken, starting line navigation ===");
  }
  uint16_t position = qtr.readLineBlack(sensorValues);

  // Count how many see black line
  int blackSensorCount = 0;
  for (uint8_t i = 1; i < 7; i++) { 
    if (sensorValues[i] > 700) {    
      blackSensorCount++;
    }
  }

  Serial.print("Position: ");
  Serial.print(position);
  Serial.print(" | blackSensorCount: ");
  Serial.print(blackSensorCount);
  Serial.print(" | Sensors: ");
  for (uint8_t i = 0; i < 8; i++) {
    Serial.print(sensorValues[i]);
    if (i < 7) Serial.print(",");
  }
  Serial.println();

  bool leftWingSeen  = (sensorValues[0] > 700 || sensorValues[1] > 700|| sensorValues[2] > 700);
  bool rightWingSeen = (sensorValues[6] > 700 || sensorValues[7] > 700|| sensorValues[5] > 700);

  if (leftWingSeen && rightWingSeen) { 
    Serial.println("ACTION: Cross-line detected!");
    
    if (!primedForStop) {
        // pause
        setMotorRaw(30, 30);       // breaking
        delay(100);
        setMotorRaw(0, 0);        
        delay(1000);               // pause delay
      
        // Execute sequence
        moveToSector(inputs.sector1);
        reachAndGrab();
        delay(1000);

        moveToSector(inputs.sector2);
        setDown();
        delay(1000);
        reachAndGrab();
        delay(1000);

        moveToSector(inputs.sector3);
        setDown();
        delay(1000);

        moveToSector(0);
      // Drive forward delay
      setMotorRaw(CRUISE_PWM, CRUISE_PWM);
      delay(400); 
      
      primedForStop = true; //flag to stop the car 
      
    } else {
      //delay
      setMotorRaw(CRUISE_PWM, CRUISE_PWM);
      delay(300);
      //stop 
      while (true) {
       
        setMotorRaw(0, 0); //infinite loop
        delay(1000);
      }
    }
    return; 
  }

  
  // Check if line is lost
  bool lostLine = true;
  for (uint8_t i = 0; i < SensorCount; i++) {
    if (sensorValues[i] > 200) {
      lostLine = false;
      break;
    }
  }

  // lost line
  if (lostLine) {
    if (lastDirection == -1) {
      setMotorRaw(-TURN_PWM, TURN_PWM); 
    } else if (lastDirection == 1) {
      setMotorRaw(TURN_PWM, -TURN_PWM); 
    } else {
      setMotorRaw(40, 40);
    }
    return;
  }

  // Navigation 
  if (position < 1500) {
    Serial.println("ACTION: Turn left");
    lastDirection = -1;
    setMotorRaw(-REVERSE_PWM, TURN_PWM); 
  } 
  else if (position >= 1500 && position < 2800) {
    Serial.println("ACTION: Steer left");
    lastDirection = -1;
    setMotorRaw(0, TURN_PWM); 
  } 
  else if (position >= 2800 && position <= 4200) {
    Serial.println("ACTION: Go straight");
    setMotorRaw(CRUISE_PWM, CRUISE_PWM);
  } 
  else if (position > 4200 && position <= 5500) {
    Serial.println("ACTION: Steer right");
    lastDirection = 1;
    setMotorRaw(TURN_PWM, 0); 
  } 
  else if (position > 5500) {
    Serial.println("ACTION: Turn right");
    lastDirection = 1;
    setMotorRaw(TURN_PWM, -REVERSE_PWM);
  }

  delay(1);
}