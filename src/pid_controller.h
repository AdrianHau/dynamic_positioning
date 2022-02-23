#pragma once

#include <phidget22.h>
#include <stdio.h>
#include <time.h>
#include <iostream>
#include <Windows.h>

class PID {

	public:

		PID(double Kp, double Ki, double Kd, double setPoint);
		double calculate_output(double current_position, double setpoint);

	private:

		double kp;
		double ki;
		double kd;

		double setpoint;

		double i_term;

		clock_t previous_time;
		double previous_error;

		double current_position;
		
};