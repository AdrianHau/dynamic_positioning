#pragma once

#include <phidget22.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <iomanip>
#include <Windows.h>

class PID {

	public:

		PID(double Kp, double Ki, double Kd, double setPoint, double minOutput, double maxOutput, double dtMax);
		double calculate_output(double current_position);
		void clear_variables();
		void reset_variables();
		void print_relevant_values();

		double i_term;

	private:

		double kp;
		double ki;
		double kd;

		double setpoint;

		double min_output;
		double max_output;

		double p_term;
		double d_term;

		clock_t previous_time;
		clock_t current_time;
		double dt;
		double dt_max;
		
		double current_error;
		double previous_error;
		double de;

		double output;
		
};
