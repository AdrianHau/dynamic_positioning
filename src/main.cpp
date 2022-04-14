#pragma once

#include <thread>

#include "pid_controller.h"
#include "resources.h"


void run_pid_controller(PID &pid, bool &is_finished, PhidgetVoltageRatioInputHandle &position_channel, PhidgetRCServoHandle &rcServo0,
	double min_position, double max_position, double &setpoint_in_percentage) {
	/*
	*	Main loop. Threaded to allow for altering the setpoint in real time.
	*/

	// Initialize necessary variables. Timer starts on initialization.
	double current_position, position_in_percentage, elapsed_time, output;
	Timer timer;

	pid.set_initial_i_term_value(75);

	do {

		PhidgetVoltageRatioInput_getVoltageRatio(position_channel, &current_position);
		output = pid.calculate_output(current_position);
		PhidgetRCServo_setTargetPosition(rcServo0, output);
		PhidgetRCServo_setEngaged(rcServo0, 1);

		elapsed_time = timer.get_elapsed_time();
		
		position_in_percentage = voltage_ratio_to_percentage(current_position, min_position, max_position);
		print_relevant_values(elapsed_time, position_in_percentage, setpoint_in_percentage, output, pid.get_i_term());
		write_to_results_file(elapsed_time, position_in_percentage, setpoint_in_percentage);

	} while (!is_finished);
}

void user_input_management(PID &pid, bool &is_finished, double &percentage, double min_position, double max_position, double &setpoint) {

	int choice;
	while (!is_finished) {
		std::cout <<	"1. Finish run\n" <<
						"2. Change setpoint\n";
		std::cin >> choice;
		switch (choice) {
			case 1: 
				is_finished = true;
				break;

			case 2:
				std::cout << "Select wanted setpoint (must be between 0 and 1):\n";
				std::cin >> percentage;
				setpoint = percentage_to_setpoint(percentage, min_position, max_position);
				pid.change_setpoint(setpoint);
				break;
		}
	}
}

int main() {
	/* 
	*/

	// Phidget setup
	PhidgetVoltageRatioInputHandle position_channel;
	PhidgetVoltageRatioInput_create(&position_channel);

	Phidget_setChannel((PhidgetHandle)position_channel, 4);
	Phidget_openWaitForAttachment((PhidgetHandle)position_channel, 5000);

	PhidgetRCServoHandle rcServo0;
	PhidgetRCServo_create(&rcServo0);

	Phidget_openWaitForAttachment((PhidgetHandle)rcServo0, 5000);


	// Clear results.txt file
	remove("dp_results.txt");


	// Determine setpoint and initialize PID controller instance
	double min_position, max_position, setpoint, percentage = 0.5;

	initialize_setup(position_channel, rcServo0, min_position, max_position);
	setpoint = percentage_to_setpoint(percentage, min_position, max_position);

	double Kp = 450, Ki = 120, Kd = 20;
	double minimum_thrust = 120, maximum_thrust = 150;
	double dt_max = 0.1;

	PID pid(Kp, Ki, Kd, setpoint, minimum_thrust, maximum_thrust, dt_max);

	std::cout << "Minimum position [V]: " << min_position << "\nMaximum position [V]: " << max_position << "\nSetpoint [V]: " << setpoint << "\n";
	system("pause");

	// testing when not connected to phidgets
	min_position = 0.03;
	max_position = 0.134;


	// Start main loop. Terminates on user input.
	bool is_finished = false;
	std::thread main_loop(run_pid_controller, std::ref(pid), std::ref(is_finished), std::ref(position_channel), std::ref(rcServo0), min_position, max_position, std::ref(percentage));
	std::thread user_management(user_input_management, std::ref(pid), std::ref(is_finished), std::ref(percentage), min_position, max_position, std::ref(setpoint));
	
	user_management.join();
	main_loop.join();


	// Deactivate phidgets
	Phidget_close((PhidgetHandle)position_channel);
	PhidgetVoltageRatioInput_delete(&position_channel);
	
	Phidget_close((PhidgetHandle)rcServo0);
	PhidgetRCServo_delete(&rcServo0);


	// Write to .vtf for visualization in GLview
	write_to_vtf_file();

}
















/*

// Initialize necessary variables. Timer starts on initialization.
double current_position, output, elapsed_time, max_runtime = 20;
Timer timer;


pid.i_term = 75; // temporary fix for integral buildup

do {

	PhidgetVoltageRatioInput_getVoltageRatio(position_channel, &current_position);
	std::cout << "Current position: " << std::fixed << std::setprecision(4) << current_position << " | ";

	output = pid.calculate_output(current_position);
	std::cout << std::fixed << std::setprecision(0) << "Thrust output: " << output << " | ";

	pid.print_relevant_values();

	PhidgetRCServo_setTargetPosition(rcServo0, output);
	PhidgetRCServo_setEngaged(rcServo0, 1);

	elapsed_time = timer.get_elapsed_time();

	/* testing variable setpoint
	if (elapsed_time > max_runtime / 2) {
		percentage = 0.8;
		pid.setpoint = min_position + percentage * (max_position - min_position);
	}


	write_to_results_file(elapsed_time, current_position, pid.get_current_error());

} while (elapsed_time <= max_runtime);

*/