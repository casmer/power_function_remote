/**
 * A Legoino example to control a power functions (IR)
 * motor or LED on the blue and red port on channel 0 of the
 * IR receiver
 * 
 * (c) Copyright 2020 - Cornelius Munz
 * Released under MIT License
 * 
 */

#include <Arduino.h>
#include "PowerFunctions.h"
#include  <millisDelay.h>

static constexpr unsigned long PRINT_DELAY_MS = 3000;

millisDelay printDelay;

int analogXPin = A6; // Analog pin connected to a potentiometer
int analogYPin = A7; // Analog pin connected to a potentiometer
int xValue = 0; // Variable to store the read value
int yValue = 0; // Variable to store the read value

int xValueScaled = 0; // Variable to store the scaled read value
int yValueScaled = 0; // Variable to store the scaled read value

int leftSpeed = 50; // initial speed
int rightSpeed = 50; // initial speed

// create a power functions instance
PowerFunctions powerFunctions(12, 0); //Pin 12, Channel 0

void setup()
{
  printDelay.start(PRINT_DELAY_MS);
  Serial.begin(115200);
}

PowerFunctionsPwm speed_lookup[] = {
  PowerFunctionsPwm::FLOAT,  // 0 - FLOAT
  PowerFunctionsPwm::REVERSE7,  // 7 - REVERSE1
  PowerFunctionsPwm::REVERSE6,  // 6 - REVERSE2
  PowerFunctionsPwm::REVERSE5,  // 5 - REVERSE3
  PowerFunctionsPwm::REVERSE4,  // 4 - REVERSE4
  PowerFunctionsPwm::REVERSE3,  // 3 - REVERSE5
  PowerFunctionsPwm::REVERSE2,  // 2 - REVERSE6
  PowerFunctionsPwm::REVERSE1,  // 1 - REVERSE7
  PowerFunctionsPwm::BRAKE,  // 8 - BRAKE
  PowerFunctionsPwm::FORWARD1,  // 9 - FORWARD1
  PowerFunctionsPwm::FORWARD2, //10 - FORWARD2
  PowerFunctionsPwm::FORWARD3, //11 - FORWARD3
  PowerFunctionsPwm::FORWARD4, //12 - FORWARD4
  PowerFunctionsPwm::FORWARD5, //13 - FORWARD5
  PowerFunctionsPwm::FORWARD6, //14 - FORWARD6
  PowerFunctionsPwm::FORWARD7  //15 - FORWARD7
};

void PrintSpeed(PowerFunctionsPwm speed)
{
  switch (speed)
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

PowerFunctionsPwm speedToPowerFunctionsPwm(int analogValue)
{
  // deadzone
  if (analogValue > 45 && analogValue < 55)
  {
    return PowerFunctionsPwm::BRAKE;
  }
  int speed = map(analogValue, -100, 100, 0, 16);
  if (speed >= 16)
  {
    speed = 15;
  } else if (speed <= 0)
  {
    speed = 1;
  }

  
  return speed_lookup[speed];
}


int scaleAnalogValue(int analogValue)
{
  // deadzone
  if (analogValue > 490 && analogValue < 530)
  {
    return 0;
  }
  int speed = map(analogValue, 0, 1023, -100, 100);
  
  return speed;
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
  //Serial.print( static_cast<int>(leftSpeed)); 
  Serial.print(" | Right Speed: [");
  Serial.print(rightSpeed);
  Serial.print("] ");
  PrintSpeed(speedToPowerFunctionsPwm(rightSpeed));
  Serial.println();
  //Serial.println(static_cast<int>(rightSpeed));

  }
  if (!printDelay.isRunning())
  {
    printDelay.start(PRINT_DELAY_MS);
  }
}

void translateJoystickToSpeed(int& leftSpeed, int& rightSpeed, int x, int y)
{
  float x_raw, y_raw, X, Y, V, W;
  x_raw = static_cast<float>(x);
  y_raw = static_cast<float>(y);

  //Scale the value (no scaling right now)
  X = x_raw;
  Y = y_raw;
  //get the number
  V =(100-abs(X)) * (Y/100) + Y;
  
  W= (100-abs(Y)) * (X/100) + X;
  leftSpeed =static_cast<int>((V+W) /2);
  rightSpeed = static_cast<int>((V-W)/2);
  

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

    powerFunctions.combo_pwm(speedToPowerFunctionsPwm(leftSpeed), speedToPowerFunctionsPwm(rightSpeed));
delay(100);
  

  // delay(1000);
  // for (int i = 0; i < 110; i += 10)
  // {
  //   PowerFunctionsPwm pwm = powerFunctions.speedToPwm(i);
  //   powerFunctions.single_pwm(PowerFunctionsPort::RED, pwm);
  //   Serial.print("single_pwm | speed: ");
  //   Serial.print(i);
  //   Serial.print(" pwm: ");
  //   Serial.println((uint8_t)pwm, HEX);
  //   delay(1000);
  // }

  // for (int i = 0; i < 7; i++)
  // {
  //   Serial.println("single_decrement");
  //   powerFunctions.single_decrement(PowerFunctionsPort::RED);
  //   delay(1000);
  // }

  // for (int i = 0; i < 3; i++)
  // {
  //   Serial.println("single_increment");
  //   powerFunctions.single_increment(PowerFunctionsPort::RED);
  //   delay(1000);
  // }

  // for (int i = 0; i < 10; i++)
  // {
  //   Serial.println("single_decrement");
  //   powerFunctions.single_decrement(PowerFunctionsPort::RED);
  //   delay(1000);
  // }


  // Serial.println("combo_pwm");
  // powerFunctions.combo_pwm(PowerFunctionsPwm::FORWARD2, PowerFunctionsPwm::REVERSE3);
  // delay(1000);

  // Serial.println("single_pwm BRAKE");
  

} // End of loop