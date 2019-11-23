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
float sensitivityCurve(float input)
{
	// Applies custom curve to input stick value
	float base = 0.0292 * input;
	float add = 0.6 * input;
	float output = std::pow(base, 3.0) + add;
	return output;
}

float trayCurve(float input)
{
	// Applies curve to motor up value for tray
	float c1 = (81 * input * input) - (46170 * input) + 6603225;
	float c2 = std::sqrt(c1) - (9 * input) + 2565;
	float c3 = std::cbrt(c2);
	float output = ((5 * c3) / 2.08008382305) - (100 / (1.44224957031 * c3)) + 90;
	if (input < 0)
	{
		return 127;
	}
	if (output < 30)
	{
		return 30;
	}
	return output;
}

float trayCurveV2(float input){
	float c1 = (4.58257569496 * std::sqrt((189 * input * input) - (159138 * input) + 33530549)) - (63 * input) + 26523;
	float c2 = std::cbrt(c1);
	float output = 1.04274265235 * c2 - (91.3342280793 / c2) + 90;
	if (input < 0)
	{
		return 50;
	}
	if (output < 30)
	{
		return 30;
	}
	return output;
}

void opcontrol()
{
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
	int backupVal = -23;
	int liftVal;

	intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
	intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
	lift.set_brake_mode(MOTOR_BRAKE_HOLD);

	tray.set_zero_position(-840);
	while (true)
	{
		// Checks if autonomous has been armed. If it has, disable control and if a is pressed run auton
		if (autonArmed)
		{
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && aPressed)
			{
				autonomous();
			}
			aPressed = (int)master.get_digital(pros::E_CONTROLLER_DIGITAL_A);
		}
		else
		{
			// get values of the left and right sticks
			leftStickYVal = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
			rightStickYVal = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

			// Puts both stick values through sensitivity curve function
			leftVal = sensitivityCurve(leftStickYVal);
			rightVal = sensitivityCurve(rightStickYVal);

			// Sets motors to values
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
			{
				// Ryans idea, when b is pressed do a slow straight backup
				driveBL = backupVal;
				driveFL = backupVal;
				driveBR = backupVal;
				driveFR = backupVal;
			}
			else
			{
				driveBL = leftVal;
				driveFL = leftVal;
				driveBR = rightVal;
				driveFR = rightVal;
			}

			// Set lift value based on R1 and R2
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
			{
				liftVal = 90;
				tray = 46;
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
			{
				liftVal = -90;
				tray = -46;
			}
			else
			{
				liftVal = 0;
			}

			// Set tray value based on L1 and L2 with emergency button
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
			{
				tray = 75;
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
			{
				tray = -90;
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
			{
				tray = trayCurveV2((float)tray.get_position());
				liftVal = -55;
			}
			else
			{
				tray = 0;
			}

			lift = liftVal;
			
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT)){
				tray.set_zero_position(0);
			}

			// Toggling forward and backward intake on and off with scuf buttons
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) && !upPressed)
			{
				if (intakeState == 1)
				{
					intakeState = 0;
				}
				else
				{
					intakeState = 1;
				}
			}

			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X) && !xPressed)
			{
				if (intakeState == -1)
				{
					intakeState = 0;
				}
				else
				{
					intakeState = -1;
				}
			}

			upPressed = (int)master.get_digital(pros::E_CONTROLLER_DIGITAL_UP);
			xPressed = (int)master.get_digital(pros::E_CONTROLLER_DIGITAL_X);

			// Sets motor speeds based on state
			if (intakeState == 1)
			{
				intakeL = 127;
				intakeR = 127;
			}
			else if (intakeState == -1)
			{
				intakeL = -70;
				intakeR = -70;
			}
			else
			{
				intakeL = 0;
				intakeR = 0;
			}
			
			debugOutput = std::to_string(trayCurve(800));
		}
		pros::delay(20);
	}
}
