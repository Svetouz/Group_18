#include "SIM3_Function_Library.h"

Motor::Motor(int ena, int in1, int in2, int enb, int in3, int in4) {
  ENA = ena;
  IN1 = in1;
  IN2 = in2;
  ENB = enb;
  IN3 = in3;
  IN4 = in4;
}

void Motor::init() {
  // Channel A
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Channel B
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.println("Motorshield pins initalised");
}

void Motor::set(const MOTOR_SELECT select, const MOTOR_DIRECTION direction, uint8_t speed) {
  if (select == MOTOR_LEFT || select == BOTH_MOTORS) {
    analogWrite(ENA, speed);
    if (direction == FORWARD) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
    } else if (direction == REVERSE) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
    } else { // BRAKE
      analogWrite(ENA, 255);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
    }
  } 
  if (select == MOTOR_RIGHT || select == BOTH_MOTORS) {
    analogWrite(ENB, speed);
    if (direction == FORWARD) {
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
    } else if (direction == REVERSE) {
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
    } else { // BRAKE
      analogWrite(ENB, 255);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
    }
  }
}

RGBLed::RGBLed(int red, int green, int blue) {
  RGB_LED_RED = red;
  RGB_LED_GREEN = green;
  RGB_LED_BLUE = blue;
}

void RGBLed::init() {
  pinMode(RGB_LED_RED, OUTPUT);
  pinMode(RGB_LED_GREEN, OUTPUT);
  pinMode(RGB_LED_BLUE, OUTPUT);
  
  Serial.println("RGB led pins initalised");
}

void RGBLed::setColour(const COLOUR_RGB colour) {
  // Do nothing if the color hasn't changed
  if (previousColour == colour) {
    return;
  }

  previousColour = colour;

  // Turn off all LEDs first
  bool RED_state = false, GREEN_state = false, BLUE_state = false;

  switch (colour) {
    case RED:
      RED_state = true;
      Serial.println("RGB LED: RED");
      break;

    case GREEN:
      GREEN_state = true;
      Serial.println("RGB LED: GREEN");
      break;

    case BLUE:
      BLUE_state = true;
      Serial.println("RGB LED: BLUE");
      break;

    case CYAN:
      GREEN_state = true;
      BLUE_state = true;
      Serial.println("RGB LED: CYAN");
      break;

    case MAGENTA:
      RED_state = true;
      BLUE_state = true;
      Serial.println("RGB LED: MAGENTA");
      break;

    case YELLOW:
      RED_state = true;
      GREEN_state = true;
      Serial.println("RGB LED: YELLOW");
      break;

    case OFF:
      Serial.println("RGB LED: OFF");
      break;

    default:
      RED_state = true;
      GREEN_state = true;
      BLUE_state = true;
      Serial.println("Invalid input!");
      break;
  }

  digitalWrite(RGB_LED_RED, RED_state);
  digitalWrite(RGB_LED_GREEN, GREEN_state);
  digitalWrite(RGB_LED_BLUE, BLUE_state);
}

TONTimer::TONTimer(unsigned long delayTime) {
  delay = delayTime;
  startTime = 0;
  isRunning = false;
  output = false;
}

void TONTimer::start() {
  if (!isRunning) {
    startTime = millis();
    isRunning = true;
  }
}

void TONTimer::stop() {
  isRunning = false;
  output = false;
}

bool TONTimer::getOutput() {
  if (isRunning && (millis() - startTime) >= delay) {
    output = true;
  }
  return output;
}

void TONTimer::reset() {
  isRunning = false;
  output = false;
  startTime = 0;
}

TOFFTimer::TOFFTimer(unsigned long delayTime) {
  delay = delayTime;
  startTime = 0;
  isRunning = true;
  output = true;
}

void TOFFTimer::start() {
  isRunning = true;
}

void TOFFTimer::stop() {
  if (!isRunning) {
    startTime = millis();
  }
  isRunning = false;
}

bool TOFFTimer::getOutput() {
  if (!isRunning && (millis() - startTime) >= delay) {
    output = false;
  }
  return output;
}

void TOFFTimer::reset() {
  isRunning = true;
  output = true;
  startTime = 0;
}