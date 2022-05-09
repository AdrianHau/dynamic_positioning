#pragma once

#include <thread>

#include "pid_controller.h"
#include "resources.h"


void run_pid_controller(PID &pid, bool &is_finished, PhidgetVoltageRatioInputHandle &position_channel, PhidgetRCServoHandle &rcServo0,
	double min_position, double max_position, double &setpoint_in_percentage) {
	/**
	*	Main loop. Threaded with user input management loop to allow for altering the setpoint in real time.
	*/

	// Initialize necessary variables. Timer starts on initialization.
	double current_position, position_in_percentage, elapsed_time, output;
	Timer timer;

	/*
	* Set the integral term to a user determined value to prevent
	* having to wait for the term to build up at the start of the run.
	*/
	pid.set_i_term_value(85);

	do {
		
		// Calculate and apply output based on current position
		PhidgetVoltageRatioInput_getVoltageRatio(position_channel, &current_position);
		output = pid.calculate_output(current_position);
		PhidgetRCServo_setTargetPosition(rcServo0, output);
		PhidgetRCServo_setEngaged(rcServo0, 1);

		// Get elapsed time and position in percentage for runtime printing and post-run plotting purposes
		elapsed_time = timer.get_elapsed_time();
		position_in_percentage = voltage_ratio_to_percentage(current_position, min_position, max_position);
		print_relevant_values(elapsed_time, position_in_percentage, setpoint_in_percentage, output, pid.get_i_term());
		write_to_results_file(elapsed_time, position_in_percentage, setpoint_in_percentage);

	} while (!is_finished);
}

void user_input_management(PID &pid, bool &is_finished, double &setpoint_in_percentage, double min_position, double max_position, double &setpoint) {
	/**
	* User input management function
	* Takes in user input to either terminate the current run, or alter the setpoint of the current run.
	* Press 1 in the terminal during runtime to terminate run, or 2 to alter the setpoint.
	* Can potentially be expanded in the future to allow for more functionalities, such as altering
	* the PID constants during runtime for example.
	*/

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
				std::cout << "Select wanted setpoint (must be a value between 0 and 1):\n";
				std::cin >> setpoint_in_percentage;
				setpoint = percentage_to_voltage_ratio(setpoint_in_percentage, min_position, max_position);
				pid.change_setpoint(setpoint);
				break;
		}
	}
}

int main() {

	// Phidget setup
	PhidgetVoltageRatioInputHandle position_channel;
	PhidgetVoltageRatioInput_create(&position_channel);
	
	Phidget_setChannel((PhidgetHandle)position_channel, 4);
	Phidget_openWaitForAttachment((PhidgetHandle)position_channel, 5000);

	PhidgetRCServoHandle rcServo0;
	PhidgetRCServo_create(&rcServo0);

	Phidget_openWaitForAttachment((PhidgetHandle)rcServo0, 5000);


	// Clear dp_results.txt file between each run
	remove("dp_results.txt");


	// Determine setpoint and initialize PID controller instance
	double min_position, max_position, setpoint, setpoint_in_percentage = 0.7;
	
	initialize_setup(position_channel, rcServo0, min_position, max_position);
	setpoint = percentage_to_voltage_ratio(setpoint_in_percentage, min_position, max_position);

	PID pid(210, 18, 15, setpoint, 105, 150, 0.1);

	std::cout << "Minimum position [V/Vmax]: " << min_position << "\nMaximum position [V/Vmax]: " << max_position << "\nSetpoint [V/Vmax]: " << setpoint << "\n";
	system("pause");


	// Start main loop. Terminates on user input in user_management thread.
	bool is_finished = false;
	std::thread main_loop(run_pid_controller, std::ref(pid), std::ref(is_finished), std::ref(position_channel), std::ref(rcServo0), min_position, max_position, std::ref(setpoint_in_percentage));
	std::thread user_management(user_input_management, std::ref(pid), std::ref(is_finished), std::ref(setpoint_in_percentage), min_position, max_position, std::ref(setpoint));
	

	// Join loops when the is_finished boolean is set to true in the user management thread
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