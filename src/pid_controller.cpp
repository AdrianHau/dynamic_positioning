#pragma once

#include "pid_controller.h"



PID::PID(double Kp, double Ki, double Kd, double Setpoint) {

	kp = Kp;
	ki = Ki;
	kd = Kd;

	setpoint = Setpoint;

	i_term = 0;
	
	clock_t previous_time = 0;
	double previous_error = 0;

}




double PID::calculate_output(double current_position, double setpoint) {

	double dt, de, current_error, p_term, d_term;
	clock_t current_time;

	current_time = clock();
	dt = double(current_time - previous_time) / CLOCKS_PER_SEC;
	previous_time = current_time;

	current_error = setpoint - current_position;
	de = current_error - previous_error;
	previous_error = current_error;

	p_term = kp * current_error;
	i_term += ki * (previous_error + current_error) / 2 * dt;
	d_term = kd * de / dt;

	return p_term + i_term + d_term;
}

