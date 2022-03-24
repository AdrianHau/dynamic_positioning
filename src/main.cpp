#pragma once

#include "pid_controller.h"
#include "pid_testing.h"
#include "resources.h"



int main() {

	// Position channel phidget setup
	PhidgetVoltageRatioInputHandle position_channel;
	PhidgetVoltageRatioInput_create(&position_channel);

	Phidget_setChannel((PhidgetHandle)position_channel, 3);
	Phidget_openWaitForAttachment((PhidgetHandle)position_channel, 5000);


	// RC Servo phidget setup
	PhidgetRCServoHandle rcServo0;
	PhidgetRCServo_create(&rcServo0);

	Phidget_openWaitForAttachment((PhidgetHandle)rcServo0, 5000);


	// Determine setpoint and initialize PID controller instance
	double min_position, max_position, setpoint, percentage = 0.6;
	initialize_setup(position_channel, rcServo0, min_position, max_position);
	
	setpoint = min_position + percentage*(max_position - min_position);
	PID pid(5000, 1000, 300, setpoint, 120, 150, 0.256);

	std::cout << "Minimum position: " << min_position << "\nMaximum position: " << max_position << "\nSetpoint: " << setpoint << "\n";
	system("pause");



	// Initialize necessary variables
	double current_position, output;

	int counter = 0;
	int max_iterations = 4000;
	pid.i_term = 80;
	do {

		PhidgetVoltageRatioInput_getVoltageRatio(position_channel, &current_position);
		std::cout << "Current position: " << std::fixed << std::setprecision(4) << current_position << " | ";

		output = pid.calculate_output(current_position);
		std::cout << std::fixed << std::setprecision(0) << "Thrust output: " << output << " | ";

		pid.print_relevant_values();

		PhidgetRCServo_setTargetPosition(rcServo0, output);
		PhidgetRCServo_setEngaged(rcServo0, 1);

		//Sleep(100);

		counter += 1;

	} while (counter < max_iterations);


	// Deactivate phidgets & reset PID controller
	pid.clear_variables();

	Phidget_close((PhidgetHandle)position_channel);
	PhidgetVoltageRatioInput_delete(&position_channel);
	
	Phidget_close((PhidgetHandle)rcServo0);
	PhidgetRCServo_delete(&rcServo0);


	/*Test functions*/
	//test_given_null_constants_then_zero_output();
	//test_given_kp_and_constant_error_then_constant_output();
	//test_given_kp_and_ki_and_constant_error_then_increasing_output();
	//test_given_kp_and_kd_and_constant_error_then_constant_output();
	//test_given_voltage_input_then_sensible_output();
	//general_phidget_testing();
}