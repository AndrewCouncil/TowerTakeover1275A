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

bool driverControlEnabled = true;
bool liftEnabled = true;
pros::ADIButton traySense('A');
pros::ADIPotentiometer trayPot('H');
pros::ADIPotentiometer liftPot('G');

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

	// ADD CURVE FUNCTION

	int output = 90;
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

void liftControl(void *param)
{
	int liftVal;
	int liftMidPos = 2280;
	int liftHighPos = 2700;
	int liftDownPos = 1140;
	liftEnabled = true;

	intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
	intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
	lift.set_brake_mode(MOTOR_BRAKE_HOLD);
	// debugOutput = "%d", autonType;

	if (autonType == 7)
	{
		intakeL = -127;
		intakeR = -127;
		lift = 80;
		while (liftPot.get_value() < (liftDownPos + 200))
		{
			pros::delay(20);
		}
		lift = -127;
		intakeL = 0;
		intakeR = 0;
		while (liftPot.get_value() > (liftDownPos + 50))
		{
			pros::delay(20);
		}
		lift = 0;
		lift.set_zero_position(0);
	}

	while (true)
	{
		debugOutput = "Lift Val: " + std::to_string(liftPot.get_value()) + "\nTray Val: " + std::to_string(trayPot.get_value());
		if (driverControlEnabled)
		{
			// Set lift value based on X and Y
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_UP))
			{
				liftVal = 90;
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
			{
				liftVal = -90;
			}
			else
			{
				liftVal = 0;
			}

			while (liftEnabled)
			{
				if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X) && liftPot.get_value() < liftHighPos)
				{
					// Lift HIGH Macro
					autoLED = 3;
					lift = 100;
					while (liftPot.get_value() < liftHighPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
					{
						pros::delay(20);
					}
					lift = 0;
					autoLED = 2;
				}
				else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && liftPot.get_value() < liftMidPos)
				{
					lift = 100;
					while (liftPot.get_value() < liftMidPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_X) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
					{
						pros::delay(20);
					}
					lift = 0;
					autoLED = 2;
				}
				else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && liftPot.get_value() > liftMidPos)
				{
					// Lift MID Macro for when lift is ABOVE POSITION
					autoLED = 3;
					while (liftPot.get_value() > liftMidPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_X) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
					{
						pros::delay(20);
					}
					lift = 0;
					autoLED = 2;
				}
				else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && liftPot.get_value() > 0)
				{
					// Lift DOWN Macro
					autoLED = 3;
					lift = -127;
					while (liftPot.get_value() > (liftDownPos + 50) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_X) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_UP) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_RIGHT))
					{
						pros::delay(20);
					}
					lift = 0;
					autoLED = 2;
				}
				else
				{
					// If not macro set to manual control values
					lift = liftVal;
				}
				// debugOutput = "I'm here";
				break;
			}
		}
		pros::delay(20);
	}
}
void driveTrayIntakeControl(void *param)
{
	autoLED = 2;
	// pros::Controller master(pros::E_CONTROLLER_MASTER);
	// master.print(1,0,"->%d", pros::battery::get_capacity());
	// master.rumble("...");
	float leftStickYVal;
	float rightStickYVal;
	float rightVal;
	float leftVal;
	float trayVal;
	float leftStrafeVal;
	float rightStrafeVal;
	float storedStrafeVal;
	int leftDoubleTapCount;
	int rightDoubleTapCount;
	int intakeState = 0;
	int upPressed;
	int xPressed;
	int aPressed;
	int downPressed;
	int bPressed;
	int backupVal = -58;
	int maxStrafe = 127;
	int midStrafe = 73;

	while (true)
	{
		// Checks if autonomous has been armed. If it has, disable control and if a is pressed run auton
		if (autonArmed)
		{
			driverControlEnabled = false;
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A) && aPressed)
			{
				autonomous();
			}
			aPressed = (int)master.get_digital(pros::E_CONTROLLER_DIGITAL_A);
		}
		else
		{
			driverControlEnabled = true;

			// Sets Intake motor speeds based on R! and R2
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R2))
			{
				intakeL = 127;
				intakeR = 127;
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_R1))
			{
				intakeL = -70;
				intakeR = -70;
			}
			else
			{
				intakeL = 0;
				intakeR = 0;
			}

			// Moves either Tray or Drive
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
			{
				trayVal = trayCurve((float)trayPot.get_value());
				driveFL = -trayVal;
				driveFR = -trayVal;
				driveBL = trayVal;
				driveBR = trayVal;
				intakeL.set_brake_mode(MOTOR_BRAKE_COAST);
				intakeR.set_brake_mode(MOTOR_BRAKE_COAST);
				lift.set_brake_mode(MOTOR_BRAKE_COAST);
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
			{
				trayVal = -127;
				driveFL = -trayVal;
				driveFR = -trayVal;
				driveBL = trayVal;
				driveBR = trayVal;
				intakeL.set_brake_mode(MOTOR_BRAKE_COAST);
				intakeR.set_brake_mode(MOTOR_BRAKE_COAST);
				lift.set_brake_mode(MOTOR_BRAKE_COAST);
			}
			else
			{

				// Sets drive motors to values for drive mode
				if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
				{
					// Ryans idea, when DOWN is pressed do a slow straight backup
					driveBL = backupVal;
					driveFL = backupVal;
					driveBR = backupVal;
					driveFR = backupVal;
					intakeL.set_brake_mode(MOTOR_BRAKE_COAST);
					intakeR.set_brake_mode(MOTOR_BRAKE_COAST);
					lift.set_brake_mode(MOTOR_BRAKE_COAST);
				}
				else
				{
					intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
					intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
					lift.set_brake_mode(MOTOR_BRAKE_HOLD);
					// get values of the left and right sticks
					leftStickYVal = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
					rightStickYVal = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

					// Puts both stick values through sensitivity curve function
					leftVal = sensitivityCurve(leftStickYVal);
					rightVal = sensitivityCurve(rightStickYVal);

					driveBL = leftVal;
					driveFL = leftVal;
					driveBR = rightVal;
					driveFR = rightVal;
				}
			}
			// Strafe Motor Control
			if (!master.get_digital(pros::E_CONTROLLER_DIGITAL_B) && master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
			{
				driveM = 0;
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
			{
				driveM = 80;
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
			{
				driveM = -80;
			}
			

			bPressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_B);
			downPressed = master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN);

			// if (downPressed && !master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
			// {
			// 	leftDoubleTapCount = 1;
			// }
			// if (bPressed && !master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
			// {
			// 	rightDoubleTapCount = 1;
			// }
			// if (leftDoubleTapCount > 0)
			// {
			// 	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
			// 	{
			// 		leftDoubleTapCount = 0;
			// 	}
			// 	else
			// 	{
			// 		strafeVal = 127;
			// 		doubleTapCount++;
			// 	}
			// }
			// if (rightDoubleTapCount > 0)
			// {
			// 	if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
			// 	{
			// 		rightDoubleTapCount = 0;
			// 	}
			// 	else
			// 	{
			// 		strafeVal = 127;
			// 		doubleTapCount++;
			// 	}
			// }

			// 	if ((master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN) + master.get_digital(pros::E_CONTROLLER_DIGITAL_B)) = /= 1)
			// 	{
			// 		driveM = 0;
			// 		downPressed = false;
			// 		bPressed = false;
			// 	}
			// 	else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
			// 	{
			// 		downPressed = true;
			// 		driveM = -strafeVal;
			// 	}
			// 	else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B))
			// 	{
			// 		bPressed = true;
			// 		driveM = strafeVal;
			// 	}

			// 	if (doubleTapCount > 20)
			// 	{
			// 		doubleTapCount = 0;
			// 	}
			// }
			pros::delay(20);
		}
	}
}

void opcontrol()
{
	pros::Task liftControl_task(liftControl, 0, TASK_PRIORITY_DEFAULT);

	pros::Task driveTrayIntakeControl_task(driveTrayIntakeControl, 0, TASK_PRIORITY_DEFAULT);
}
