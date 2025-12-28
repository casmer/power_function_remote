/// @copyright  Copyright (c) 2025 Casey Gregoire
///             Released under MIT License

/// @param analogValue 
/// @return 
int speedToPowerFunctionsPwm(int analogValue);

int rampFunction(int inputValue);


int scaleAnalogValue(int analogValue);

void translateJoystickToSpeed(int& leftSpeed, int& rightSpeed, int x, int y);
