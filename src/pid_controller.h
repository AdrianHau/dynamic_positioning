#pragma once

#include <iomanip>

#include <phidget22.h>

/**
* Generalized PID class implementation.
*
* Only requires a setpoint and the position relative
* to the setpoint, both given as a double.
*/
class PID {

	public:

		/**
		* PID Class constructor.
		* 
		* Description:
		*	Initializes PID parameters to the constructor input values and
		*	sets all runtime variables to zero through the clear_variables()
		*	function below.
		*
		* Parameters:
		*	Kp:			Proportional gain constant value
		*	Ki:			Integral gain constant value
		*	Kd:			Derivative gain constant value
		*	setPoint:	Reference point the controller should aim towards.
		*	minOutput:	The minimum output the controller will deliver.
		*	maxOutput:	The maximum output the controller will deliver.
		*	dtMax:		The maximum value the dt variable can have. If surpassed,
		*				the calculate_output function will return a value of zero
		*				and move on to the next iteration.
		* 
		* Returns:
		*	No return value
		*/
		PID(double Kp, double Ki, double Kd, double setPoint, double minOutput, double maxOutput, double dtMax);
		void clear_variables();

		/**
		* Calculates PID output for the current time step based on current error.
		* 
		* Description:
		*	To be used within a continuous loop. The timestep is calculated
		*	as the difference of time between each iteration of the loop
		*	using the clock() function.
		* 
		* Parameters:
		*	current_position: Current position of the object.
		* 
		* Returns:
		*	A double containing the current output value.
		*/
		double calculate_output(double current_position);

		/**
		* Changes setpoint to given value during runtime.
		*/
		void change_setpoint(double setPoint);

		/**
		* Sets integral term to wanted starting value. Useful
		* in cases where the integral term needs to build up
		* until the output reaches the minimum output value.
		*/
		void set_i_term_value(double i_term_value);

		/**
		* Returns integral term for plotting & runtime analysis.
		*/
		double get_i_term() const;


	private:

		// PID gain constants
		double kp;
		double ki;
		double kd;

		// The reference point to aim for
		double setpoint;

		// Output value constraints
		double min_output;
		double max_output;
		
		// dt value constraint
		double dt_max;

		// Runtime variables
		double p_term;
		double i_term;
		double d_term;

		clock_t previous_time;
		clock_t current_time;
		double dt;
		
		double current_error;
		double previous_error;
		double de;

		double output;
		
};

/**
* Potential future additions:
*	- Ability to alter Kp, Ki, Kd during runtime.
*	- More get_variable() or get_constant() functions
*	  similar to the get_i_term() function. 
*	- Integral windup guard
*/