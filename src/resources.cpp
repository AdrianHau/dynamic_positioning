#pragma once

#include "resources.h"

double set_setpoint(PhidgetVoltageRatioInputHandle position_channel) {

	double setpoint;

	std::cout << "\nPlace boat in wanted reference position.";
	system("pause");
	PhidgetVoltageRatioInput_getVoltageRatio(position_channel, &setpoint);
	std::cout << "Reference position has been set to: " << setpoint << "\n";

	return setpoint;

}