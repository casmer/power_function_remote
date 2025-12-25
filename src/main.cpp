/// @copyright  Copyright (c) 2025 Casey Gregoire
///             Released under MIT License

#include <Arduino.h>
#include "PowerFunctions.h"
#include <millisDelay.h>
#include "utilities.hpp"

/// @brief Delay between prints to Serial and no values have changed
static constexpr unsigned long PRINT_DELAY_MS = 3000;
/// @brief Delay object for printing
millisDelay printDelay;

int analogXPin = A6; // Analog pin connected to a potentiometer
int analogYPin = A7; // Analog pin connected to a potentiometer
int xValue = 0;      // Variable to store the read value
int yValue = 0;      // Variable to store the read value

int xValueScaled = 0; // Variable to store the scaled read value
int yValueScaled = 0; // Variable to store the scaled read value

int leftSpeed = 50;  // initial speed
int rightSpeed = 50; // initial speed

// create a power functions instance
PowerFunctions powerFunctions(12, 0); // Pin 12, Channel 0

void setup()
{
  printDelay.start(PRINT_DELAY_MS);
  Serial.begin(115200);
}


void PrintSpeed(int speed)
{
  switch (static_cast<PowerFunctionsPwm>(speed))
  {
  case PowerFunctionsPwm::FLOAT:
    Serial.print("FLOAT");
    break;
  case PowerFunctionsPwm::BRAKE:
    Serial.print("BRAKE");
    break;
  case PowerFunctionsPwm::FORWARD1:
    Serial.print("FORWARD1");
    break;
  case PowerFunctionsPwm::FORWARD2:
    Serial.print("FORWARD2");
    break;
  case PowerFunctionsPwm::FORWARD3:
    Serial.print("FORWARD3");
    break;
  case PowerFunctionsPwm::FORWARD4:
    Serial.print("FORWARD4");
    break;
  case PowerFunctionsPwm::FORWARD5:
    Serial.print("FORWARD5");
    break;
  case PowerFunctionsPwm::FORWARD6:
    Serial.print("FORWARD6");
    break;
  case PowerFunctionsPwm::FORWARD7:
    Serial.print("FORWARD7");
    break;
  case PowerFunctionsPwm::REVERSE1:
    Serial.print("REVERSE1");
    break;
  case PowerFunctionsPwm::REVERSE2:
    Serial.print("REVERSE2");
    break;
  case PowerFunctionsPwm::REVERSE3:
    Serial.print("REVERSE3");
    break;
  case PowerFunctionsPwm::REVERSE4:
    Serial.print("REVERSE4");
    break;
  case PowerFunctionsPwm::REVERSE5:
    Serial.print("REVERSE5");
    break;
  case PowerFunctionsPwm::REVERSE6:
    Serial.print("REVERSE6");
    break;
  case PowerFunctionsPwm::REVERSE7:
    Serial.print("REVERSE7");
    break;
  default:
    Serial.print(static_cast<int>(speed));
    break;
  }
}

void printValues()
{
  static int last_xValueScaled = 0;
  static int last_yValueScaled = 0;

  if (printDelay.justFinished() || last_xValueScaled != xValueScaled || last_yValueScaled != yValueScaled)
  {
    last_xValueScaled = xValueScaled;
    last_yValueScaled = yValueScaled;

    Serial.print("X: ");
    Serial.print(xValue);
    Serial.print(" | Scaled X: ");
    Serial.print(xValueScaled);
    Serial.print(" | Y: ");
    Serial.print(yValue);
    Serial.print(" | Scaled Y: ");
    Serial.println(yValueScaled);

    Serial.print("Left Speed: [");
    Serial.print(leftSpeed);
    Serial.print("] ");
    PrintSpeed(speedToPowerFunctionsPwm(leftSpeed));
    // Serial.print( static_cast<int>(leftSpeed));
    Serial.print(" | Right Speed: [");
    Serial.print(rightSpeed);
    Serial.print("] ");
    PrintSpeed(speedToPowerFunctionsPwm(rightSpeed));
    Serial.println();
    // Serial.println(static_cast<int>(rightSpeed));
  }
  if (!printDelay.isRunning())
  {
    printDelay.start(PRINT_DELAY_MS);
  }
}

// main loop
void loop()
{

  xValue = analogRead(analogXPin); // read the input pin
  yValue = analogRead(analogYPin); // read the input pin
  xValueScaled = scaleAnalogValue(xValue);
  yValueScaled = scaleAnalogValue(yValue);
  translateJoystickToSpeed(leftSpeed, rightSpeed, xValueScaled, yValueScaled);

  printValues();

  powerFunctions.combo_pwm(
      static_cast<PowerFunctionsPwm>(speedToPowerFunctionsPwm(leftSpeed)),
      static_cast<PowerFunctionsPwm>(speedToPowerFunctionsPwm(rightSpeed)));
  delay(100);

} // End of loop