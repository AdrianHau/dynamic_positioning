#pragma once

#include <phidget22.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <Windows.h>

class PID {

	public:

		PID(double Kp, double Ki, double Kd, double setPoint, double minOutput, double maxOutput);
		double calculate_output(double current_position);
		void clear_variables();

	private:

		double kp;
		double ki;
		double kd;

		double setpoint;

		double min_output;
		double max_output;

		double p_term;
		double i_term;
		double d_term;

		clock_t current_time;
		clock_t previous_time;
		double dt;
		
		double current_error;
		double previous_error;
		double de;

		double output;
		
};
