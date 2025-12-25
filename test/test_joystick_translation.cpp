/// @copyright  Copyright (c) 2025 Casey Gregoire
///             Released under MIT License

#include <gmock/gmock.h>
#include "utilities.hpp"
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