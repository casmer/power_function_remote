/// @copyright  Copyright (c) 2025 Casey Gregoire
///             Released under MIT License

#if defined(ARDUINO)
#include <Arduino.h>
#else
#include <cmath>
#endif
int map(int x, int min_input, int max_input, int min_output, int max_output)
{
return (x - min_input) * (max_output - min_output) / (max_input - min_input) + min_output;
}

int speedToPowerFunctionsPwm(int analogValue)
{
    
  // deadzone
  if (analogValue > 45 && analogValue < 55)
  {
    return 7;
  }
  int speed = map(analogValue, -100, 100, 0, 16);
  if (speed >= 16)
  {
    speed = 15;
  } else if (speed <= 0)
  {
    speed = 1;
  }

  
static int speed_lookup[] = {
    0, // 0 - FLOAT
    9, // 9 - REVERSE7
    10, // 10 - REVERSE6
    11, // 11 - REVERSE5
    12, // 12 - REVERSE4
    13, // 13 - REVERSE3
    14, // 14 - REVERSE2
    15, // 15 - REVERSE1
    8, // 8 - BRAKE
    1, // 1 - FORWARD1
    2, // 2 - FORWARD2
    3, // 3 - FORWARD3
    4, // 4 - FORWARD4
    5, // 5 - FORWARD5
    6, // 6 - FORWARD6
    7  // 7 - FORWARD7
};

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
