#include "main.h"
#include "globals.hpp"

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

// Sensitivity curve for the drive joysticks
float sensitivityCurve(float input) {
	// Applies custom curve to input stick value
	float base = 0.0292 * input;
	float add = 0.6 * input;
	float output = std::pow(base, 3.0) + add;
	// double output = input;
	// Makes sure value is inside [-127,127]
	// if(output > 127){
	// 	output = 127;
	// }
	// if(output < -127){
	// 	output = -127;
	// }
	// Makes a deadzone
	// if(std::abs(output) <= 5){
	// 	output = 0;
	// }
	return output;
}

void opcontrol() {
	// pros::Controller master(pros::E_CONTROLLER_MASTER);
	// master.print(1,0,"->%d", pros::battery::get_capacity());
	// master.rumble("...");
	float leftStickYVal;
	float rightStickYVal;
	float rightVal;
	float leftVal;
	int intakeState = 0;
	int upPressed;
	int xPressed;
	int aPressed;
	while (true) {
		// Checks if autonomous has been armed. If it has, disable control and if a is pressed run auton
		if(autonArmed){
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && aPressed){
				autonomous();
			} 
			aPressed = (int) master.get_digital(pros::E_CONTROLLER_DIGITAL_A);
		}
		else{
			// get values of the left and right sticks
			leftStickYVal = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
			rightStickYVal = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
			
			// Puts both stick values through sensitivity curve function
			leftVal  = sensitivityCurve(leftStickYVal);
			rightVal = sensitivityCurve(rightStickYVal);

			// Sets motors to values
			driveBL = leftVal;
			driveFL = leftVal;
			driveBR = rightVal;
			driveFR = rightVal;
			debugOutput = std::to_string(leftVal) + "\n" + std::to_string(rightVal);
			
			// Set lift value based on R1 and R2
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
				lift = 127;
			}
			else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
				lift = -127;
			}
			else{
				lift = 0;
			}

			// Set tray value based on L1 and L2
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L1)){
				tray = 127;
			}
			else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_L2)){
				tray = -127;
			}
			else{
				tray = 0;
			}

			// Toggling forward and backward intake on and off with scuf buttons
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) && !upPressed){
				if(intakeState == 1){
					intakeState = 0;
				}
				else{
					intakeState = 1;
				}
			}
			
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_X) && !xPressed){
				if(intakeState == -1){
					intakeState = 0;
				}
				else{
					intakeState = -1;
				}
			}

			upPressed = (int) master.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
			xPressed  = (int) master.get_digital(pros::E_CONTROLLER_DIGITAL_X);
			
			// Sets motor speeds based on state
			if(intakeState == 1){
				intakeL = 127;
				intakeR = 127;
			}
			else if(intakeState == -1){
				intakeL = -127;
				intakeR = -127;
			}
			else{
				intakeL = 0;
				intakeR = 0;
			}
		}
		pros::delay(20);
	}
}
