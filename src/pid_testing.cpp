//#pragma once
//
//#include "pid_testing.h"
//
//
//void test_given_null_constants_then_zero_output() {
//	/* 
//	* Should expect zero output when all of the constants
//	* are equal to zero.
//	*/
//	double kp = 0.0;
//	double ki = 0.0;
//	double kd = 0.0;
//	double setpoint = 0.0;
//	double curr_pos = -1.0;
//	PID pid(kd, ki, kd, setpoint);
//	double output = pid.calculate_output(curr_pos, setpoint);
//
//	int counter = 0;
//	int max_iterations = 20;
//	while (counter<max_iterations) {
//		output = pid.calculate_output(curr_pos, setpoint);
//		std::cout << output << '\n';
//		Sleep(500);
//		counter += 1;
//	}
//	// OK 
//}
//
//void test_given_kp_and_constant_error_then_constant_output() {
//	/*
//	* Should expect output to remain constant with constant error
//	* with given kp and ki = kd = 0.
//	*/
//
//	double kp = 10.0;
//	double ki = 0.0;
//	double kd = 0.0;
//	double setpoint = 0.0;
//	double curr_pos = -1.0;
//	PID pid(kp, ki, kd, setpoint);
//	double output = pid.calculate_output(curr_pos, setpoint);
//
//	int counter = 0;
//	int max_iterations = 20;
//	while (counter < max_iterations) {
//		output = pid.calculate_output(curr_pos, setpoint);
//		std::cout << output << '\n';
//		Sleep(500);
//		counter += 1;
//	}
//	// OK
//}
//
//void test_given_kp_and_ki_and_constant_error_then_increasing_output() {
//	/*
//	* Should expect output to increase when we add integral effect to the
//	* constant error case.
//	*/
//	double kp = 10.0;
//	double ki = 40.0;
//	double kd = 0.0;
//	double setpoint = 0.0;
//	double curr_pos = -1.0;
//	PID pid(kp, ki, kd, setpoint);
//	double output = pid.calculate_output(curr_pos, setpoint);
//
//	int counter = 0;
//	int max_iterations = 20;
//	while (counter < max_iterations) {
//		output = pid.calculate_output(curr_pos, setpoint);
//		std::cout << output << '\n';
//		Sleep(500);
//		counter += 1;
//	}
//	// OK
//}
//
//void test_given_kp_and_kd_and_constant_error_then_constant_output() {
//	/*
//	* Should expect output to remain constant despite derivative effect
//	* due to constant error.
//	*/
//	double kp = 10.0;
//	double ki = 0.0;
//	double kd = 10.0;
//	double setpoint = 0.0;
//	double curr_pos = -1.0;
//	PID pid(kp, ki, kd, setpoint);
//	double output = pid.calculate_output(curr_pos, setpoint);
//
//	int counter = 0;
//	int max_iterations = 20;
//	while (counter < max_iterations) {
//		output = pid.calculate_output(curr_pos, setpoint);
//		std::cout << output << '\n';
//		Sleep(500);
//		counter += 1;
//	}
//	// OK
//}
//
//static void CCONV onVoltageRatioChange(PhidgetVoltageRatioInputHandle ch, void* ctx, double voltageRatio) {
//	//printf("VoltageRatio: %lf\n", voltageRatio);
//}
//
//void general_phidget_testing() {
//
//	// Position measuring setup
//	PhidgetVoltageRatioInputHandle voltageRatioInput3;
//
//	PhidgetVoltageRatioInput_create(&voltageRatioInput3);
//
//	Phidget_setChannel((PhidgetHandle)voltageRatioInput3, 3);
//
//	PhidgetVoltageRatioInput_setOnVoltageRatioChangeHandler(voltageRatioInput3, onVoltageRatioChange, NULL);
//
//	Phidget_openWaitForAttachment((PhidgetHandle)voltageRatioInput3, 5000);
//
//	// RC Servo setup
//	PhidgetRCServoHandle rcServo0;
//
//	PhidgetRCServo_create(&rcServo0);
//
//	Phidget_openWaitForAttachment((PhidgetHandle)rcServo0, 5000);
//
//
//	// PID Controller setup
//	getchar(); // Set reference here
//
//	double current_position, output, setpoint;
//	double kp = 200.0;
//	double ki = 20.0;
//	double kd = 20.0;
//
//	PhidgetVoltageRatioInput_getVoltageRatio(voltageRatioInput3, &setpoint);
//
//	PID pid(kp, ki, kd, setpoint);
//
//
//
//	// Run "program"
//	int counter = 0;
//	int max_iterations = 10;
//	while (counter < max_iterations) {
//		PhidgetVoltageRatioInput_getVoltageRatio(voltageRatioInput3, &current_position);
//		std::cout << "Current position: " << current_position;
//
//		output = pid.calculate_output(current_position, setpoint)*100;
//
//		if (output < 110) {
//			output = 110;
//		}
//
//		if (output > 169) {
//			output = 169;
//		}
//
//		std::cout << " Thrust output:    " << output << '\n';
//
//		PhidgetRCServo_setTargetPosition(rcServo0, output);
//		PhidgetRCServo_setEngaged(rcServo0, 1);
//
//		Sleep(256);
//		counter += 1;
//	}
//
//	//Wait until Enter has been pressed before exiting
//	getchar();
//
//	Phidget_close((PhidgetHandle)voltageRatioInput3);
//	PhidgetVoltageRatioInput_delete(&voltageRatioInput3);
//
//	Phidget_close((PhidgetHandle)rcServo0);
//	PhidgetRCServo_delete(&rcServo0);
//}