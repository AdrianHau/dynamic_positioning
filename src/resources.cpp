#pragma once

#include "resources.h"


Timer::Timer() {

	start_time = clock();

}

double Timer::get_elapsed_time() {

	return double(clock() - start_time) / CLOCKS_PER_SEC;

}

void initialize_setup(PhidgetVoltageRatioInputHandle position_channel, 
	PhidgetRCServoHandle rcServo, double &min_position, double &max_position) {
	/**
	* RC Boat setup
	* 
	* Description:
	*	Stores minimum and maximum voltage ratio values at the two ends
	*	of the water tank so that the setpoint can be chosen as a
	*	percentage of the water tank distance, i.e. 0 being at one end
	*	and 1 at the other.
	* 
	* Parameters:
	*	position_channel:	PhidgetInterfaceKit 8/8/8 voltage ratio input handle
	*	rsServo:			PhidgetServo 1-Motor thrust output handle
	*	min_position:		Minimum voltage ratio value of the tank 
	*	max_position:		Maximum voltage ratio value of the tank
	* 
	* Returns:
	*	No return value
	*/

	// Get minimum voltage ratio position and move to maximum position
	PhidgetVoltageRatioInput_getVoltageRatio(position_channel, &min_position);
	PhidgetRCServo_setTargetPosition(rcServo, 135);
	PhidgetRCServo_setEngaged(rcServo, 1);
	std::cout << "Press any key when boat in maximum position.\n";
	system("pause");

	// Get maximum voltage ratio position and return to minimum position
	PhidgetVoltageRatioInput_getVoltageRatio(position_channel, &max_position);
	PhidgetRCServo_setTargetPosition(rcServo, 120);
	PhidgetRCServo_setEngaged(rcServo, 1);
	std::cout << "Press any key when boat in starting position.\n";
	system("pause");

	// Set thrust to zero until further command given
	PhidgetRCServo_setTargetPosition(rcServo, 0);
	PhidgetRCServo_setEngaged(rcServo, 1);

}



void write_to_results_file(double time, double position, double setpoint) {
	/**
	* Writes relevant information at a given time step to txt file
	* for analyzation and visualization.
	* 
	* Parameters:
	*	time:		Time elapsed since start of the run
	*	position:	Current position of the vessel
	*	setpoint:	Reference point to aim for
	*				(can be altered during runtime)
	* 
	* Returns:
	*	No return value
	*/

	FILE *results_file = fopen("dp_results.txt", "a");
	fprintf(results_file, "%lf %lf %lf\n", time, position, setpoint);
	fclose(results_file);

}

void write_to_vtf_file() {
	/**
	* Reads relevant information from the generated txt file and translates
	* the logged position into vtf file to create animation in GLview Inova.
	* 
	* Returns:
	*	No return value
	*/

	FILE* vtf_file = fopen("dp_results.vtf", "w");

	fprintf(vtf_file, "*VTF-1.00\n\n");

	// Nodes for boat
	fprintf(vtf_file, "!nodes for boat:\n");
	fprintf(vtf_file, "*NODES 1\n");
	fprintf(vtf_file, "0. -0.05 0.\n");
	fprintf(vtf_file, "0.2 0. 0.\n");
	fprintf(vtf_file, "0. 0.05 0.\n");
	fprintf(vtf_file, "0. -0.05 0.05\n");
	fprintf(vtf_file, "0.2 0. 0.05\n");
	fprintf(vtf_file, "0. 0.05 0.05\n\n");

	// Nodes for water tank
	fprintf(vtf_file, "!nodes for water tank:\n");
	fprintf(vtf_file, "*NODES 2\n");
	fprintf(vtf_file, "0. -0.2 0.\n");
	fprintf(vtf_file, "1. -0.2 0.\n");
	fprintf(vtf_file, "1. 0.2 0.\n");
	fprintf(vtf_file, "0. 0.2 0.\n\n\n");

	// Set up elements based on nodes above
	fprintf(vtf_file, "*ELEMENTS 1\n");
	fprintf(vtf_file, "%%NODES #1\n");
	fprintf(vtf_file, "%%PENTAHEDRONS\n");
	fprintf(vtf_file, "1 2 3 4 5 6\n\n");

	fprintf(vtf_file, "*ELEMENTS 2\n");
	fprintf(vtf_file, "%%NODES #2\n");
	fprintf(vtf_file, "%%COLORS 0, 0, 255\n");
	fprintf(vtf_file, "%%QUADS\n");
	fprintf(vtf_file, "1 2 3 4\n\n");

	// Group the geometries (elements)
	fprintf(vtf_file, "*GLVIEWGEOMETRY 1\n");
	fprintf(vtf_file, "%%ELEMENTS\n");
	fprintf(vtf_file, "1, 2\n\n");

	// Read results.txt file and write results (position) to vtf file
	FILE* txt_file = fopen("dp_results.txt", "r");
	double time, position, setpoint;
	int i = 1, j;

	while (feof(txt_file) == false) {

		fscanf(txt_file, "%lf %lf %lf", &time, &position, &setpoint);

		fprintf(vtf_file, "*RESULTS %d\n", i);
		fprintf(vtf_file, "%%DIMENSION 3\n");
		fprintf(vtf_file, "%%PER_NODE #1\n");
		fprintf(vtf_file, "%lf 0 0\n", position);
		fprintf(vtf_file, "%lf 0 0\n", position);
		fprintf(vtf_file, "%lf 0 0\n", position);
		fprintf(vtf_file, "%lf 0 0\n", position);
		fprintf(vtf_file, "%lf 0 0\n", position);
		fprintf(vtf_file, "%lf 0 0\n\n\n", position);

		++i;

	}

	fprintf(vtf_file, "*GLVIEWVECTOR 1\n");
	fprintf(vtf_file, "%%NAME \"Displacement\"\n");

	for (j = 1;j < i;++j) {

		fprintf(vtf_file, "%%STEP %d\n", j);
		fprintf(vtf_file, "%d\n", j);

	}

	fclose(txt_file);
	fclose(vtf_file);

}


double voltage_ratio_to_percentage(double voltage_ratio, double min_voltage_ratio, double max_voltage_ratio) {
	/**
	* Transforms voltage ratio value, i.e. the position of the boat, to a percentage value
	* based on minimum and maximum voltage ratio values of the setup.
	*/

	return ((voltage_ratio - min_voltage_ratio) / (max_voltage_ratio - min_voltage_ratio));

}

double percentage_to_voltage_ratio(double percentage, double min_voltage_ratio, double max_voltage_ratio) {
	/**
	* Calculates setpoint in unit voltage ratio based on given percentage of the
	* distance between minimum and maximum voltage ratio values of the setup.
	*/

	return (min_voltage_ratio + percentage * (max_voltage_ratio - min_voltage_ratio));

}

void print_relevant_values(double elapsed_time, double position, double setpoint, double thrust, double i_term) {
	/**
	* Prints relevant runtime values to the console.
	* Mainly for debugging.
	*/

	std::cout << std::fixed << std::setprecision(3) << "t = " << elapsed_time << " [s] | ";
	std::cout << std::fixed << std::setprecision(3) << "position = " << position << " [-] | ";
	std::cout << std::fixed << std::setprecision(2) << "setpoint = " << setpoint << " [-] | ";
	std::cout << std::fixed << std::setprecision(1) << "thrust =  " << thrust << " [-] | ";
	std::cout << std::fixed << std::setprecision(1) << "i_term = " << i_term << " [-] \n";

}
