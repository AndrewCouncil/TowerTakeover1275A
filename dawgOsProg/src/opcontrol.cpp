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
void opcontrol() {
	// pros::Controller master(pros::E_CONTROLLER_MASTER);
	// master.print(1,0,"->%d", pros::battery::get_capacity());
	// master.rumble("...");
	int aPressed;
	while (true) {
		if(autonArmed){
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && aPressed){
				autonomous();
			}
			aPressed = (int) master.get_digital(pros::E_CONTROLLER_DIGITAL_A);
		}
		else{
			driveFL = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
			driveBL = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
			driveFR = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
			driveBR = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);
			
			if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R1)){
				lift = 127;
			}
			else if(master.get_digital(pros::E_CONTROLLER_DIGITAL_R2)){
				
			}
			else{

			}
		}
		pros::delay(20);
	}
}
