#pragma once

#include "pid_controller.h"


PID::PID(double Kp, double Ki, double Kd, double setPoint, double minOutput, double maxOutput, double dtMax) {

	kp = Kp;
	ki = Ki;
	kd = Kd;

	setpoint = setPoint;

	min_output = minOutput;
	max_output = maxOutput;

	dt_max = dtMax;

	clear_variables();

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

double PID::calculate_output(double current_position) {

	current_time = clock();
	dt = double(current_time - previous_time) / CLOCKS_PER_SEC;
	previous_time = current_time;

	current_error = setpoint - current_position;
	de = current_error - previous_error;
	previous_error = current_error;

	/*	
	* Return 0 and consequentially move on to next iteration if dt is abnormally large.
	* Usually happens on first iteration as previous_time is initialized to 0, which
	* makes  dt = (current_time - previous_time)  large. Will look for a better fix later.
	*/
	if (dt > dt_max) { return 0; }
	
	p_term = kp * current_error;
	i_term += ki * (previous_error + current_error) / 2 * dt;
	d_term = kd * de / dt;

	output = (p_term + i_term + d_term);

	if (output < min_output) { output = min_output; }
	else if (output >= max_output) { output = max_output; }

	return output;

}

void PID::change_setpoint(double setPoint) {

	setpoint = setPoint;

}

void PID::set_i_term_value(double i_term_value) {

	i_term = i_term_value;

}

double PID::get_i_term() const {

	return i_term;

}