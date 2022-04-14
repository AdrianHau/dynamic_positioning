#pragma once

#include <iomanip>

#include <phidget22.h>


class PID {
	/* 
	*	Generalized PID class implementation. Only requires
	*	a setpoint and the position relative to the setpoint
	*	given as a float/double.
	* 
	* 
	*	GENERALIZED PID - Not dependent on "external factors",
	*	purely dependent on setpoint and current_position.
	*/

	public:

		PID(double Kp, double Ki, double Kd, double setPoint, double minOutput, double maxOutput, double dtMax);
		void clear_variables();
		double calculate_output(double current_position);
		void change_setpoint(double setPoint);
		void set_initial_i_term_value(double i_term_value);
		double get_i_term() const;


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

		clock_t previous_time;
		clock_t current_time;
		double dt;
		double dt_max;
		
		double current_error;
		double previous_error;
		double de;

		double output;
		
};
