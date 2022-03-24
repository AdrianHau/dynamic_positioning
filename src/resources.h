#pragma once

#include <phidget22.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <Windows.h>

double set_setpoint(PhidgetVoltageRatioInputHandle position_channel);
void initialize_setup(PhidgetVoltageRatioInputHandle position_channel, PhidgetRCServoHandle rcServo, double &min_position, double &max_position);