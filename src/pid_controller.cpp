#pragma once

#include "pid_controller.h"



PID::PID(double Kp, double Ki, double Kd, double setPoint, double minOutput, double maxOutput) {

	kp = Kp;
	ki = Ki;
	kd = Kd;

	setpoint = setPoint;

	min_output = minOutput;
	max_output = maxOutput;

	clear_variables();

}




double PID::calculate_output(double current_position) {

	current_time = clock();
	dt = double(current_time - previous_time) / CLOCKS_PER_SEC;
	previous_time = current_time;

	current_error = setpoint - current_position;
	de = current_error - previous_error;
	previous_error = current_error;

	p_term = kp * current_error;
	i_term += ki * (previous_error + current_error) / 2 * dt; // Trapesmetoden foreløpig, teste andre? Har det mye å si?
	d_term = kd * de / dt;

	output = (p_term + i_term + d_term) * 100; // Skalering??????

	if (output < min_output) { output = 0; }
	if (output >= max_output) { output = 150; }

	return output;
}


void PID::clear_variables() {

	p_term = 0;
	i_term = 0;
	d_term = 0;

	current_time = 0;
	previous_time = 0;
	dt = 0;

	current_error = 0;
	previous_error = 0;
	de = 0;

	output = 0;

}