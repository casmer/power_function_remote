// Copyright (c) 2025 Cotsbotics
// Author: Casey Gregoire <caseyg@lalosoft.com>


#include <gmock/gmock.h>




void translateJoystickToSpeed(int& leftSpeed, int& rightSpeed, int x, int y)
{
  float x_raw, y_raw, X, Y, V, W;
  x_raw = static_cast<float>(x);
  y_raw = static_cast<float>(y);

    //Scale the value (no scaling right now)
    X = x_raw;
    Y = y_raw;
  //get the number
  std::cout << "x_raw: " << x_raw << " y_raw: " << y_raw << std::endl;
  V =(100-abs(X)) * (Y/100) + Y;
  
  W= (100-abs(Y)) * (X/100) + X;
  std::cout << "V: " << V << " W: " << W << std::endl;
  leftSpeed =static_cast<int>((V+W) /2);
  rightSpeed = static_cast<int>((V-W)/2);
  

}


TEST(joystick_converter, idle)
{

    int leftSpeed = 0;
    int rightSpeed = 0;
    int x = 0;
    int y = 0;  
    translateJoystickToSpeed(leftSpeed, rightSpeed, x, y);
    EXPECT_EQ(leftSpeed, 0);
    EXPECT_EQ(rightSpeed, 0);
    

}



TEST(joystick_converter, full_forward)
{

  
    int leftSpeed = 0;
    int rightSpeed = 0;
    int x = 0;
    int y = 100;  
    translateJoystickToSpeed(leftSpeed, rightSpeed, x, y);
    EXPECT_EQ(leftSpeed, 100);
    EXPECT_EQ(rightSpeed, 100);
    

}


TEST(joystick_converter, full_reverse)
{

  
    int leftSpeed = 0;
    int rightSpeed = 0;
    int x = 0;
    int y = -100;  
    translateJoystickToSpeed(leftSpeed, rightSpeed, x, y);
    EXPECT_EQ(leftSpeed, -100);
    EXPECT_EQ(rightSpeed, -100);
    

}



TEST(joystick_converter, full_right)
{
    int leftSpeed = 0;
    int rightSpeed = 0;
    int x = 100;
    int y = 0;  
    translateJoystickToSpeed(leftSpeed, rightSpeed, x, y);
    EXPECT_EQ(leftSpeed, 100);
    EXPECT_EQ(rightSpeed, -100);
}


TEST(joystick_converter, full_left)
{
    int leftSpeed = 0;
    int rightSpeed = 0;
    int x = -100;
    int y = 0;  
    translateJoystickToSpeed(leftSpeed, rightSpeed, x, y);
    EXPECT_EQ(leftSpeed, -100);
    EXPECT_EQ(rightSpeed, 100);
}