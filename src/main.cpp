#pragma once

#include "pid_controller.h"
#include "pid_testing.h"
#include "resources.h"


static void CCONV onVoltageRatioChange(PhidgetVoltageRatioInputHandle ch, void* ctx, double voltageRatio) {
	printf("VoltageRatio: %lf\n", voltageRatio);
}

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
	double setpoint = set_setpoint(position_channel);
	PID pid(200, 20, 20, setpoint);

	std::cout << "Place boat in wanted starting position.\n";
	system("pause");

	// Initialize necessary variables
	double current_position, output;

	int counter = 0;
	int max_iterations = 30;
	do {
		// Get current position
		PhidgetVoltageRatioInput_getVoltageRatio(position_channel, &current_position);
		std::cout << "Current position: " << current_position;

		output = pid.calculate_output(current_position, setpoint) * 100; // scaling??????????????
		std::cout << " Thrust output:    " << output << '\n';

		PhidgetRCServo_setTargetPosition(rcServo0, output);
		PhidgetRCServo_setEngaged(rcServo0, 1);

		Sleep(256);

		counter += 1;

	} while (counter < max_iterations);






	// Deactivate phidgets
	Phidget_close((PhidgetHandle)position_channel);
	PhidgetVoltageRatioInput_delete(&position_channel);

	Phidget_close((PhidgetHandle)rcServo0);
	PhidgetRCServo_delete(&rcServo0);


	//test_given_null_constants_then_zero_output();
	//test_given_kp_and_constant_error_then_constant_output();
	//test_given_kp_and_ki_and_constant_error_then_increasing_output();
	//test_given_kp_and_kd_and_constant_error_then_constant_output();
	//test_given_voltage_input_then_sensible_output();
	//general_phidget_testing();
}