#pragma once

#include "resources.h"


// delete most likely
double set_setpoint(PhidgetVoltageRatioInputHandle position_channel) {

	double setpoint;

	std::cout << "\nPlace boat in wanted reference position.";
	system("pause");
	PhidgetVoltageRatioInput_getVoltageRatio(position_channel, &setpoint);
	std::cout << "Reference position has been set to: " << setpoint << "\n";

	return setpoint;

}


void initialize_setup(PhidgetVoltageRatioInputHandle position_channel, PhidgetRCServoHandle rcServo, double &min_position, double &max_position) {

	// Set minimum position and move to maximum position
	PhidgetVoltageRatioInput_getVoltageRatio(position_channel, &min_position);
	PhidgetRCServo_setTargetPosition(rcServo, 135);
	PhidgetRCServo_setEngaged(rcServo, 1);


	// Set maximum position and return to minimum position
	std::cout << "Press any key when boat in maximum position.\n";
	system("pause");
	PhidgetVoltageRatioInput_getVoltageRatio(position_channel, &max_position);
	PhidgetRCServo_setTargetPosition(rcServo, 120);
	PhidgetRCServo_setEngaged(rcServo, 1);
	std::cout << "Press any key when boat in starting position.\n";
	system("pause");

	// Set thrust to zero
	PhidgetRCServo_setTargetPosition(rcServo, 0);
	PhidgetRCServo_setEngaged(rcServo, 1);

}