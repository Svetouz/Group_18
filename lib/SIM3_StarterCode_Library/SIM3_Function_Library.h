#ifndef SIM3_FUNCTION_LIBRARY_H
#define SIM3_FUNCTION_LIBRARY_H

#include <Arduino.h>

/**
 * @brief Motor direction for wheel rotation.
 *
 * Used with Motor::set() to drive the motor forward, reverse, or to brake.
 */
typedef enum {REVERSE, FORWARD, BRAKE} MOTOR_DIRECTION;

/**
 * @brief Motor selector for choosing which motor(s) to control.
 */
typedef enum {MOTOR_LEFT, MOTOR_RIGHT, BOTH_MOTORS} MOTOR_SELECT;

/**
 * @brief Predefined RGB colors for the RGB LED.
 */
typedef enum {RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, OFF} COLOUR_RGB;

/**
 * @brief Motor control wrapper for L298N style driver.
 *
 * Controls two motors (left/right) with PWM speed and direction pins.
 */
class Motor {
private:
  // Channel A: left motor
  int ENA;   // PWM pin
  int IN1;
  int IN2;
  
  // Channel B: right motor
  int ENB;   // PWM pin
  int IN3;
  int IN4;

public:
  /**
   * @brief Construct a new Motor object
   * 
   * @param ena PWM enable pin for motor A (left)
   * @param in1 Direction pin 1 for motor A
   * @param in2 Direction pin 2 for motor A
   * @param enb PWM enable pin for motor B (right)
   * @param in3 Direction pin 1 for motor B
   * @param in4 Direction pin 2 for motor B
   */
  Motor(int ena, int in1, int in2, int enb, int in3, int in4);
  
  /**
   * @brief Initialize the motor driver pins.
   *
   * Sets pin modes for PWM and direction pins. Must be called in setup().
   */
  void init();
  
  /**
   * @brief Set motor(s) direction and speed.
   *
   * @param select Which motor(s) to control (LEFT, RIGHT, or BOTH).
   * @param direction Motor direction (FORWARD, REVERSE, BRAKE).
   * @param speed PWM speed value (0-255).
   */
  void set(const MOTOR_SELECT select, const MOTOR_DIRECTION direction, uint8_t speed);
};

/**
 * @brief RGB LED driver for three separate PWM pins.
 */
class RGBLed {
private:
  int RGB_LED_RED;
  int RGB_LED_GREEN;
  int RGB_LED_BLUE;
  
  int previousColour = OFF;

public:
  /**
   * @brief Construct a new RGBLed object
   *
   * @param red Pin controlling red LED channel
   * @param green Pin controlling green LED channel
   * @param blue Pin controlling blue LED channel
   */
  RGBLed(int red, int green, int blue);
  
  /**
   * @brief Initialize the RGB LED pins.
   *
   * Sets pin modes for the RGB LED channels.
   */
  void init();
  
  /**
   * @brief Set the RGB LED to a predefined color.
   *
   * @param colour One of the COLOUR_RGB values (RED, GREEN, BLUE, etc).
   */
  void setColour(const COLOUR_RGB colour);
};

/**
 * @brief On-delay timer (TON)
 *
 * Starts timing when start() is called; getOutput() becomes true after the
 * configured delay has elapsed while running.
 */
class TONTimer {
private:
  unsigned long delay;      // delay in milliseconds
  unsigned long startTime;  // millis() when timer started
  bool isRunning;           // true when timer is active
  bool output;              // current output state

public:
  /**
   * @brief Construct a new TONTimer
   * @param delayTime Milliseconds until output becomes true after start()
   */
  TONTimer(unsigned long delayTime);

  /** @brief Start or restart the timer (output false until elapsed). */
  void start();

  /** @brief Stop the timer and reset output to false. */
  void stop();

  /**
   * @brief Get the current output state.
   * @return true if delay elapsed while running
   */
  bool getOutput();

  /** @brief Reset the timer to stopped state with output=false. */
  void reset();
};

/**
 * @brief Off-delay timer (TOFF)
 *
 * Keeps output true while running. When stop() is called, output remains true
 * until delay has elapsed.
 */
class TOFFTimer {
private:
  unsigned long delay;      // delay in milliseconds before output turns false
  unsigned long startTime;  // millis() when stop() was called
  bool isRunning;           // true when timer is active (output stays true)
  bool output;              // current output state

public:
  /**
   * @brief Construct a new TOFFTimer
   * @param delayTime Milliseconds the output stays true after stop() is called
   */
  TOFFTimer(unsigned long delayTime);

  /** @brief Start or restart the timer (output stays true). */
  void start();

  /**
   * @brief Stop the timer and begin the off-delay.
   *
   * Output remains true until delay has elapsed.
   */
  void stop();

  /**
   * @brief Get the current output state.
   * @return true when output is active, false after the off-delay expires
   */
  bool getOutput();

  /** @brief Reset to initial running state (output=true). */
  void reset();
};

#endif
