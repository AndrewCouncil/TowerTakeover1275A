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
	pros::Motor left_mtr(1);
	pros::Motor right_mtr(2);
	while (true) {
		// pros::lcd::print(0, "%d %d %dtest", (pros::lcd::read_buttons() & LCD_BTN_LEFT) >> 2,
		//                  (pros::lcd::read_buttons() & LCD_BTN_CENTER) >> 1,
		//                  (pros::lcd::read_buttons() & LCD_BTN_RIGHT) >> 0);
		// int left = master.get_analog(ANALOG_LEFT_Y);
		// int right = master.get_analog(ANALOG_RIGHT_Y);
		
		// left_mtr = left;
		// right_mtr = right;
		if((pros::lcd::read_buttons() & LCD_BTN_RIGHT) > 0){
			right_mtr = 100; 
			left_mtr = 100;
		}
		else{
			right_mtr = 0;
			left_mtr = 0;
		}
		pros::lcd::print(0, "%d", (pros::lcd::read_buttons() & LCD_BTN_RIGHT));
		pros::delay(20);
	}
}
