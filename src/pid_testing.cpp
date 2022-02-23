#pragma once

#include "pid_testing.h"

void test_pid_implementation() {

	double kp = 10.0;
	double ki = 30.0;
	double kd = 5.0;
	double setpoint = 0.0;
	double curr_pos = -1.0;
	PID pid(kd, ki, kd, setpoint);
	double output = pid.calculate_output(curr_pos, setpoint);
	while (1) {
		output = pid.calculate_output(curr_pos, setpoint);
		std::cout << output << '\n';
		Sleep(500);
	}
}
