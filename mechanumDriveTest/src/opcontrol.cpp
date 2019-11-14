#include "main.h"

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	pros::Motor left_omni (13, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
	pros::Motor right_omni(15, MOTOR_GEARSET_18, true,  MOTOR_ENCODER_DEGREES);
	pros::Motor left_mech (4, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
	pros::Motor right_mech(3, MOTOR_GEARSET_18, true,  MOTOR_ENCODER_DEGREES);
	int X2 = 0, Y1 = 0, X1 = 0, threshold = 15, test = 0;

	while (true) {
		// 
		pros::lcd::print(0, "check%d %d %d %d %d", master.get_analog(ANALOG_RIGHT_X), master.get_analog(ANALOG_LEFT_X), master.get_analog(ANALOG_LEFT_Y), master.get_analog(ANALOG_RIGHT_Y), test);

		if(abs(master.get_analog(ANALOG_LEFT_Y)) > threshold)
    		Y1 = master.get_analog(ANALOG_LEFT_Y);
  		else
		    Y1 = 0;
		//Create "deadzone" for X1/Ch4
		if(abs(master.get_analog(ANALOG_LEFT_X)) > threshold)
			X1 = master.get_analog(ANALOG_LEFT_X);
		else
			X1 = 0;
		//Create "deadzone" for X2/Ch1
		if(abs(master.get_analog(ANALOG_RIGHT_X)) > threshold)
			X2 = master.get_analog(ANALOG_RIGHT_X);
		else
			X2 = 0;

		//Remote Control Commands
		
		right_mech = Y1 - X2 - X1;   // Front Right
		right_omni =  Y1 - X2 + 0.75*X1;// + X1;  // Back Right
		left_mech = test = Y1 + X2 + X1; // Front Left
		left_omni =  Y1 + X2 - 0.75*X1;// - X1; //Back Left

		pros::delay(20);
	}
}
