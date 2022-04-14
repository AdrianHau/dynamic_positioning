#pragma once

#include <iomanip>
#include <iostream>

#include <phidget22.h>


class Timer {
	/*	Timer to keep track of time during runtime.
	*
	*	Implemented as a class so that potential additional functionality
	*	can be added in the future if wanted. Timer starts ticking upon
	*	initialization of the timer instance.
	* 
	*/

	public:

		Timer();
		double get_elapsed_time();

	private:

		clock_t start_time;

};

void initialize_setup(PhidgetVoltageRatioInputHandle position_channel, PhidgetRCServoHandle rcServo, double &min_position, double &max_position);
void write_to_results_file(double elapsed_time, double position, double setpoint);
void write_to_vtf_file();
double voltage_ratio_to_percentage(double current_voltage, double min_voltage, double max_voltage);
double percentage_to_setpoint(double percentage, double min_voltage, double max_voltage);
void print_relevant_values(double elapsed_time,	double position, double setpoint, double thrust, double i_term);
