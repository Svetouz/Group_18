#include <Arduino.h>
#include <QTRSensors.h>
#include "../lib/SIM3_StarterCode_Library/SIM3_Function_Library.h"

//How to calibrate QTR sensors:
  // Calibration Phase Instructions:
  // During calibration (about 10 seconds), the RGB LED will turn YELLOW to
  // indicate that calibration is in progress.
  //
  // While the LED is yellow, move the sensors so that EACH sensor is exposed
  // to both the lightest and darkest surfaces it will encounter during normal
  // operation. For a line-following robot, slowly slide the sensors back and
  // forth across the black line and the lighter background.
  //
  // This allows the system to learn the correct range for "dark" and "light."
  // If the sensors do not see both extremes during calibration, the readings
  // will be inaccurate and overall performance will be poor.

QTRSensors qtr;
const uint8_t IR_PIN_ARRAY[] = {52, 50, 48, 46, 53, 51, 49, 47}; // Pins connected to the IR sensors
const int IR_EMITTER_PIN = 2; // Pin to control IR LED emitter
const int IR_SENSOR_COUNT = sizeof(IR_PIN_ARRAY) / sizeof(IR_PIN_ARRAY[0]) ;


//const int SENSOR_THRESHOLD = 500; // Treshold for digital line detection
const int SENSOR_THRESHOLD_LOW = 250; // Treshold for digital line detection
const int SENSOR_THRESHOLD_HIGH = 750; // Treshold for digital line detection

uint16_t sensorValues[IR_SENSOR_COUNT];
uint16_t line_position; // variable to store the line position read by the sensors, 0 to 7000, where 3500 is the center of the line

// Car constants (tune these as needed for your specific cart)
const uint8_t TURNING_SPEED = 60;
const uint8_t NORMAL_SPEED = 6;
const int PAUSE_DELAY = 0;
const int STOP_DELAY = 450;
const int CORNER_DELAY = 150;
const int PAUSE_DURATION = 5000; 

Motor motor(2, 3, 4, 7, 6, 5);
//RGBLed led(53, 49, 51);
TONTimer stopTimer(250);   // 250 milliseconds on-delay
TOFFTimer pauseTimer(250); // 250 millisecond off-delay

//functions
void initIRsensorArray();
void initCalibrationIRsensor();
void printIRsensorValues();
bool all_sensor_HIGH(); // Return true when all IR sensors read black (HIGH values)
bool all_sensor_LOW(); // Return true when all IR sensors read white (LOW values)
bool pause_detected();
bool stop_detected();
bool corner_left_detected();
bool corner_right_detected();
bool line_around_center();

void Run_Arm_Code(){
  // Add your robot arm movement code here for picking up the object.
  // This code will be combined with the driving code so the robot car
  // can complete the course and pick up objects.
  //
  // IMPORTANT:
  // - First test your arm movement code separately.
  // - Make sure it works correctly on its own before adding it here.
  // - Only integrate it into this function once it has been tested and is working as expected.
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("SIM3 Startercode");
  Serial.println("Initialising...");
  motor.init();
  //led.init();

  initIRsensorArray();
  //led.setColour(YELLOW); // indicate calibration in progress
  initCalibrationIRsensor(); // in future we want to have calibration on a button press and possibly save the calibration data
  //led.setColour(OFF); // turn off LED after calibration
  Serial.println("Initialisation complete");
  //led.setColour(GREEN); // indicate ready to start
}

void loop() {
  // put your main code here, to run repeatedly:
  line_position = qtr.readLineBlack(sensorValues);
  //printIRsensorValues(); // serial print may be removed later to improve performance

  if (pause_detected()) {
    // keep driving for a short time to get in center of pause zone
    // because the pause symbol interupts the line the code stop correcting position and asume the car is centered
    motor.set(BOTH_MOTORS, FORWARD, NORMAL_SPEED);
    delay(PAUSE_DELAY); 

    //led.setColour(BLUE); 
    motor.set(BOTH_MOTORS, BRAKE, 255); // brake with full power to stop as quickly as possible
    delay(5000);
    Run_Arm_Code();
    //led.setColour(GREEN); 

    Serial.println("pause over");
  }
  
  if (stop_detected()) {
    // keep driving for a short time to get in center of stop zone
    // because the stop symbol interupts the line the code stop correcting position and asume the car is centered
    motor.set(BOTH_MOTORS, FORWARD, NORMAL_SPEED);
    delay(STOP_DELAY); // keep moving to get in center of stop zone
    //led.setColour(RED); 
    motor.set(BOTH_MOTORS, BRAKE, 255);
    Serial.println("stopped");
    delay(2500); // keep the car stopped for a short time before stopping the program; adjust as needed

    //exit(0); // stop the program
  }
  
  if (corner_left_detected()) {
    // drive forward a for a fixed time to get in the corner at the rotation point.
    // the start turning to the left till the line is around the center again.
    Serial.println("corner left detected");
    motor.set(BOTH_MOTORS, FORWARD, NORMAL_SPEED);
    delay(CORNER_DELAY); // this will not work for sharper corners, so higher than 90 degree corners
    motor.set(BOTH_MOTORS, BRAKE, 255);
    delay(500);

    do {
      //keep reading sensors to know when to stop turning if the line is on center again
      motor.set(MOTOR_LEFT, FORWARD, TURNING_SPEED); // just go forward for now
      motor.set(MOTOR_RIGHT, REVERSE, TURNING_SPEED); // just go forward for now
      delay(10); 
      motor.set(MOTOR_LEFT, FORWARD, TURNING_SPEED/2); // just go forward for now
      motor.set(MOTOR_RIGHT, REVERSE, TURNING_SPEED/2); // just go forward for now
      delay(10);
      qtr.read(sensorValues);
    } while (!line_around_center()); // keep turning until the line is around the center again

    motor.set(BOTH_MOTORS, BRAKE, 255);
    delay(1000);
  }

  if (corner_right_detected()) {
    // drive forward a for a fixed time to get in the corner at the rotation point.
    // the start turning to the right till the line is around the center again.
    Serial.println("corner right detected");
    motor.set(BOTH_MOTORS, FORWARD, NORMAL_SPEED);
    delay(CORNER_DELAY); // this will not work for sharper corners, so higher than 90 degree corners
    motor.set(BOTH_MOTORS, BRAKE, 255);
    delay(500);

    do {
      motor.set(MOTOR_LEFT, REVERSE, TURNING_SPEED); // just go forward for now
      motor.set(MOTOR_RIGHT, FORWARD, TURNING_SPEED); // just go forward for now
      delay(10); 
      motor.set(MOTOR_LEFT, REVERSE, TURNING_SPEED/2); // just go forward for now
      motor.set(MOTOR_RIGHT, FORWARD, TURNING_SPEED/2); // just go forward for now
      delay(10);
      qtr.read(sensorValues);
    } while (!line_around_center()); // keep turning until the line is around the center again

    motor.set(BOTH_MOTORS, BRAKE, 255);
    delay(1000);
  }

  if (line_position != 3500 || all_sensor_HIGH() || all_sensor_LOW()) { // line is not in the center, or we lost the line (all sensors LOW)
    if (line_position > 3500) { // turning left
      if (line_position > 4750) line_position = 4750;
      int var1 = map(line_position, 3500, 4750, NORMAL_SPEED, 0);
      int var2 = map(line_position, 3500, 4750, NORMAL_SPEED, 255);
      motor.set(MOTOR_LEFT, FORWARD, var1);
      motor.set(MOTOR_RIGHT, FORWARD, var2);
    } else { // tuning right
      if (line_position < 2250) line_position = 2250;
      int var3 = map(line_position, 3500, 2250, NORMAL_SPEED, 255);
      int var4 = map(line_position, 3500, 2250, NORMAL_SPEED, 0);
      motor.set(MOTOR_LEFT, FORWARD, var3);
      motor.set(MOTOR_RIGHT, FORWARD, var4);
    }
  } else { // line in center. just go forward
    motor.set(BOTH_MOTORS, FORWARD, NORMAL_SPEED);
  }
}


//Function definitions here:
void initIRsensorArray(){
  // configure the sensors
  qtr.setTypeRC();
  qtr.setSensorPins(IR_PIN_ARRAY, IR_SENSOR_COUNT);
  qtr.setEmitterPin(IR_EMITTER_PIN);

  Serial.print("IR sensor array initalised with pins: ");
  for (uint8_t i = 0; i < IR_SENSOR_COUNT; i++) {
    Serial.print(IR_PIN_ARRAY[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println("Total number of sensors: " + String(IR_SENSOR_COUNT));
}

void initCalibrationIRsensor(){
  // Calibrate the sensors
  Serial.println("Calibrating IR sensor array...");
  for (uint16_t i = 0; i < 400; i++) {
    qtr.calibrate();
  }

  // print the calibration minimum values measured when emitters were on
  for (uint8_t i = 0; i < IR_SENSOR_COUNT; i++) {
    Serial.print(qtr.calibrationOn.minimum[i]);
    Serial.print(' ');
  }
  Serial.println();

  // print the calibration maximum values measured when emitters were on
  for (uint8_t i = 0; i < IR_SENSOR_COUNT; i++) {
    Serial.print(qtr.calibrationOn.maximum[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();

  Serial.println("IR sensor array calibrated");
}

void printIRsensorValues(){
  for (uint8_t i = 0; i < IR_SENSOR_COUNT; i++)
  {
    Serial.print(sensorValues[i]);
    Serial.print('\t');
  }
  Serial.println(line_position);
  delay(50); // adjust delay as needed for performance vs. readability of serial output
}

bool all_sensor_HIGH() {
  for (uint8_t i = 0; i < IR_SENSOR_COUNT; i++) {
    if (sensorValues[i] < SENSOR_THRESHOLD_LOW) { // due to bad course surface this was more reliable
    // if (sensorValues[i] < SENSOR_THRESHOLD_HIGH) {
      return false;
    }
  }
  return true;
}

bool all_sensor_LOW() {
  for (uint8_t i = 0; i < IR_SENSOR_COUNT; i++) {
    if (sensorValues[i] > SENSOR_THRESHOLD_HIGH) { // due to bad course surface this was more reliable
    // if (sensorValues[i] < SENSOR_THRESHOLD_LOW) {
      return false;
    }
  }
  return true;
}

bool pause_detected(){ 
  // We want to keep the pause detection stable: it should only fire after
  // seeing a full-black zone followed by a full-white zone.
  // The TOFF timer is used to ensure the black zone lasts long enough.
  static bool sawBlack = false;

  // NOTE: sensor values are inverted: black gives HIGH readings, white gives LOW readings
  if (all_sensor_HIGH()) {
    // first, see the full-black pause symbol
    sawBlack = true;
    pauseTimer.start(); // keep timer output true while we are in black
    return false;
  }

  if (all_sensor_LOW()) { // we are in the white zone; only trigger pause if we previously saw black

    if (!sawBlack) {
      pauseTimer.reset();
      return false;
    }

    // start off-delay on the timer; it will go false after the delay
    pauseTimer.stop();
    if (!pauseTimer.getOutput()) {
      // White zone was present long enough after the black zone
      sawBlack = false;
      Serial.println("pause detected");
      return true;
    }
    return false;
  }

  // neither full black nor full white -> reset state
  sawBlack = false;
  pauseTimer.reset();
  return false;
}

bool stop_detected(){ 
  if (all_sensor_HIGH()) {
    // start (or continue) counting while the stop symbol is visible
    stopTimer.start();

    if (stopTimer.getOutput()) {
      Serial.println("stop");
      return true;
    }
  } else {
    // if the stop symbol is gone, reset the timer so it must be detected again
    stopTimer.reset();
  }
  return false;
}

bool corner_left_detected(){ // not the cleanest way but easy to understand
  // 90-degree left corner detection:
  if (sensorValues[0] < SENSOR_THRESHOLD_HIGH) return false;
  if (sensorValues[3] < SENSOR_THRESHOLD_HIGH) return false;
  if (sensorValues[4] < SENSOR_THRESHOLD_HIGH) return false;
  if (sensorValues[6] > SENSOR_THRESHOLD_LOW) return false;
  if (sensorValues[7] > SENSOR_THRESHOLD_LOW) return false;
  return true;
}

bool corner_right_detected(){ // not the cleanest way but easy to understand
  // 90-degree right corner detection:
  if (sensorValues[7] < SENSOR_THRESHOLD_HIGH) return false;
  if (sensorValues[4] < SENSOR_THRESHOLD_HIGH) return false;
  if (sensorValues[3] < SENSOR_THRESHOLD_HIGH) return false;
  if (sensorValues[1] > SENSOR_THRESHOLD_LOW) return false;
  if (sensorValues[0] > SENSOR_THRESHOLD_LOW) return false;
  return true;
}

bool line_around_center(){ 
  if(sensorValues[3] >= SENSOR_THRESHOLD_HIGH || sensorValues[4] >= SENSOR_THRESHOLD_HIGH){
    return true;
  } else{
    return false;
  }
}