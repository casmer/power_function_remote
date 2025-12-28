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
int invertJoystickXPin = A1; // Analog pin to read if X axis should be inverted
int invertJoystickYPin = A2; // Analog pin to read if Y axis should be inverted
bool invertJoystickX = false;
bool invertJoystickY = false;
int xValue = 0;      // Variable to store the read value
int yValue = 0;      // Variable to store the read value

int xValueScaled = 0; // Variable to store the scaled read value
int yValueScaled = 0; // Variable to store the scaled read value

int xValueRamped = 0; // Variable to store the ramped x value
int yValueRamped = 0; // Variable to store the ramped y value


int leftSpeed = 50;  // initial speed
int rightSpeed = 50; // initial speed

// create a power functions instance
PowerFunctions powerFunctions(12, 3); // Pin 12, Channel 3

void setup()
{
  printDelay.start(PRINT_DELAY_MS);
  Serial.begin(115200);
  pinMode(invertJoystickXPin, INPUT_PULLUP);
  pinMode(invertJoystickYPin, INPUT_PULLUP);
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

    Serial.print("X");
    if (invertJoystickX)
    {
      Serial.print("(inverted): ");
    }
    else
    {
      Serial.print(": ");
    }
    Serial.print(xValue);
    Serial.print(" | Scaled X: ");
    Serial.print(xValueScaled);
    
    Serial.print(" | Ramped X: ");
    Serial.println(xValueRamped);
    Serial.print("Y");
    if (invertJoystickY)
    {
      Serial.print("(inverted): ");
    }
    else
    {
      Serial.print(": ");
    }
    Serial.print(yValue);
    Serial.print(" | Scaled Y: ");
    Serial.print(yValueScaled);

    Serial.print(" | Ramped Y: ");
    Serial.println(yValueRamped);

    Serial.print("Left Speed: [");
    Serial.print(leftSpeed);
    Serial.print("] ");
    PrintSpeed(static_cast<int>(powerFunctions.speedToPwm(leftSpeed)));
    // Serial.print( static_cast<int>(leftSpeed));
    Serial.print(" | Right Speed: [");
    Serial.print(rightSpeed);
    Serial.print("] ");
    PrintSpeed(static_cast<int>(powerFunctions.speedToPwm(rightSpeed)));
    Serial.println();
    // Serial.println(static_cast<int>(rightSpeed));
  }
  if (!printDelay.isRunning())
  {
    printDelay.start(PRINT_DELAY_MS);
  }
}

void getJoystickValues()
{

 //static  int xValue, yValue = 0; // previous states or each joystick input, default to zero
constexpr int minChange = 5; // minimum amount of change that must be seen to determine that the stick has moved
constexpr int numSamples = 20; // how many samples to take when reading the joystick

 int x = 0, y = 0, z = 0, sum_x = 0, sum_y = 0; // start with all values at zero 

  // collect the required number of samples and add to a sum
  for(int i = 0; i<=numSamples; i++)
  {
    sum_x += analogRead(analogXPin);
    sum_y += analogRead(analogYPin);
  }

  // find the mean average of the samples
  x = sum_x / numSamples;
  y = sum_y / numSamples;

  // if the average of either axis is greater than the minimum required change, or the button state has changed, display the values
  if (abs(x - xValue) > minChange || abs(y - yValue) > minChange)
  { 
    //Serial.printf("Stick: %d,%d Δ: %d,%d / btn:%d \n", x, x, abs(x-xValue), abs(y-yValue), z);

    // save the current values as the new 'previous' value to compare on th next loop
    xValue = x;
    yValue = y;
    
  }
}

// main loop
void loop()
{
getJoystickValues();
  invertJoystickX = false; //digitalRead(invertJoystickXPin) == LOW;
  invertJoystickY = false; //digitalRead(invertJoystickYPin) == LOW;
  // xValue = analogRead(analogXPin); // read the input pin
  // if (invertJoystickX)
  // {
  //   xValue = 1023 - xValue;
  // }
  // yValue = analogRead(analogYPin); // read the input pin
  // if (invertJoystickY)
  // {
  //   yValue = 1023 - yValue;
  // }
  xValueScaled = scaleAnalogValue(xValue);
  yValueScaled = scaleAnalogValue(yValue);
  xValueRamped = rampFunction(xValueScaled);
  yValueRamped = rampFunction(yValueScaled);
  translateJoystickToSpeed(leftSpeed, rightSpeed, xValueRamped, yValueRamped);
 leftSpeed = -leftSpeed; // Invert left motor
  printValues();

  powerFunctions.combo_pwm(
      static_cast<PowerFunctionsPwm>(powerFunctions.speedToPwm(leftSpeed)),
      static_cast<PowerFunctionsPwm>(powerFunctions.speedToPwm(rightSpeed)));
  delay(30);

} // End of loop