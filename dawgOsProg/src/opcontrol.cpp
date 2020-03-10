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
bool liftTrayEnabled = true;
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

float trayCurveV2(float input)
{
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

float trayCurveV3(float input)
{
	// float output = 0.083333333 * (std::sqrt(input * input - 890 * input + 222025) - input + 1045);
	float output;
	if (input >= 450)
	{
		output = -1 * (0.000178571428571 * (input - 700) * (input - 700)) + 40;
	}
	else
	{
		output = (0.000178571428571 * (input - 700) * (input - 700)) + 40;
	}
	// All that work just to eventually make it quadratic lol

	if (input < 0)
	{
		return 50;
	}
	if (output < 23)
	{
		return 23;
	}
	return output;
}

float trayCurveV4(float input)
{
	// float output = 0.083333333 * (std::sqrt(input * input - 890 * input + 222025) - input + 1045);
	float output;
	output = (-0.000015 * input * input) + 127;
	// All that work just to eventually make it quadratic lol
	// debugOutput = std::to_string(output);
	if (input < 0)
	{
		return 127;
	}
	if (output < 60)
	{
		return 60;
	}
	return output;
}

float trayCurveV5(float input)
{
	float output;
	float c = input - 420; //heh
	output = -0.0000000000000000027 * std::pow(c, 6) + 127;

	// debugOutput = std::to_string(output);
	if (input < 594)
	{
		return 127;
	}
	if (output < 60)
	{
		return 60;
	}
	return output;
}

float trayCurveV6(float input)
{
	float output;
	// float c = 200 * input - 261000;
	// output = -0.7 * std::pow(c, (1 / 3)) + 88;

	// // debugOutput = std::to_string(output);
	// if (input < 400)
	// {
	// 	return 127;
	// }
	// if (output > 127)
	// {
	// 	return 127;
	// }
	// if (output < 40)
	// {
	// 	return 40;
	// }
	if (input < 1660)
	{
		output = 127;
	}
	else
	{
		output = 50;
	}
	return output;
}

void liftTrayControl(void *param)
{
	int liftVal;
	int trayVal;
	int trayDownPos = 490;
	int trayMacroPos = trayDownPos + 860;
	int liftMidPos = 2280;
	int liftHighPos = 2700;
	int liftMostlyDownPos = 2370;
	int liftDownPos = 1140;
	liftTrayEnabled = true;

	intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
	intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
	lift.set_brake_mode(MOTOR_BRAKE_HOLD);
	tray.set_brake_mode(MOTOR_BRAKE_COAST);
	// debugOutput = "%d", autonType;

	if (autonType == 7)
	{
		tray = 127;
		lift = 80;
		while (liftPot.get_value() < (liftDownPos + 200))
		{
			pros::delay(20);
		}
		lift = -127;
		tray = -127;
		while (liftPot.get_value() > (liftDownPos + 50))
		{
			pros::delay(20);
		}
		lift = 0;
		lift.set_zero_position(0);
		while (trayPot.get_value() > (trayDownPos + 30))
		{
			pros::delay(20);
		}
		tray = 0;
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

			// Set tray value based on L1 and L2 with emergency button
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_LEFT))
			{
				trayVal = 90;
				autoLED = 2;
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
			{
				trayVal = -127;
				autoLED = 2;
				intakeL.set_brake_mode(MOTOR_BRAKE_COAST);
				intakeR.set_brake_mode(MOTOR_BRAKE_COAST);
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1))
			{
				trayVal = trayCurveV6((float)trayPot.get_value());
				liftVal = 0;
				autoLED = 3;
				intakeL.set_brake_mode(MOTOR_BRAKE_COAST);
				intakeR.set_brake_mode(MOTOR_BRAKE_COAST);
			}
			else
			{
				trayVal = 0;
				autoLED = 2;
				intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
				intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
			}
			while (liftTrayEnabled)
			{
				if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X) && liftPot.get_value() < liftHighPos)
				{
					intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
					intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
					// Lift HIGH Macro
					autoLED = 3;
					if (trayPot.get_value() < trayMacroPos)
					{
						tray = 127;
						lift = 100;
						while (trayPot.get_value() < trayMacroPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						tray = 0;
					}
					else if (trayPot.get_value() > trayMacroPos)
					{
						tray = -127;
						while (trayPot.get_value() > trayMacroPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						tray = 0;
						lift = 100;
					}
					while (liftPot.get_value() < liftHighPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					lift = 0;
					autoLED = 2;
				}
				else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && liftPot.get_value() < liftMidPos)
				{
					intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
					intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
					// Lift MID Macro for when lift is BELOW POSITION
					autoLED = 3;
					if (trayPot.get_value() < trayMacroPos)
					{
						tray = 127;
						lift = 100;
						while (trayPot.get_value() < trayMacroPos && liftPot.get_value() < liftMidPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						lift = 0;
						while (trayPot.get_value() < (trayMacroPos - 100) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						tray = 0;
						lift = 100;
					}
					else if (trayPot.get_value() > trayMacroPos)
					{
						tray = -127;
						while (trayPot.get_value() > trayMacroPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						tray = 0;
						lift = 100;
					}
					while (liftPot.get_value() < liftMidPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					lift = 0;
					autoLED = 2;
				}
				else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && liftPot.get_value() > liftMidPos)
				{
					intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
					intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
					// Lift MID Macro for when lift is ABOVE POSITION
					autoLED = 3;
					if (trayPot.get_value() < trayMacroPos)
					{
						tray = 127;
						while (trayPot.get_value() < trayMacroPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						tray = 0;
						lift = -100;
					}
					else if (trayPot.get_value() > trayMacroPos)
					{
						tray = -127;
						while (trayPot.get_value() > trayMacroPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						tray = 0;
						lift = -100;
					}
					while (liftPot.get_value() > liftMidPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					lift = 0;
					autoLED = 2;
				}
				else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B) && liftPot.get_value() > 0)
				{
					intakeL.set_brake_mode(MOTOR_BRAKE_COAST);
					intakeR.set_brake_mode(MOTOR_BRAKE_COAST);
					// Lift DOWN Macro
					if (trayPot.get_value() < trayMacroPos && liftPot.get_value() > (liftHighPos - 170))
					{
						tray = 127;
						while (trayPot.get_value() < trayMacroPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
					}

					tray = 0;
					autoLED = 3;
					lift = -127;
					while (liftPot.get_value() > liftMostlyDownPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					lift.set_brake_mode(MOTOR_BRAKE_COAST);
					lift = -90;
					tray = -127;
					while (trayPot.get_value() > (trayDownPos + 30) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					tray = 0;
					tray.set_zero_position(0);
					while (liftPot.get_value() > (liftDownPos + 50) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					lift = 0;
					lift.set_brake_mode(MOTOR_BRAKE_HOLD);
					autoLED = 2;
				}
				else
				{
					// If not macro set to manual control values
					tray = trayVal;
					lift = liftVal;
				}
				// debugOutput = "I'm here";
				break;
			}

			if (traySense.get_value() != 0)
			{
				tray.set_zero_position(0);
			}
		}
		pros::delay(20);
	}
}
void driveIntakeControl(void *param)
{
	autoLED = 2;
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
	int backupVal = -58;

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

			// if (master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
			// {
			// 	liftTrayEnabled = false;
			// }
			// else
			// {
			// 	liftTrayEnabled = true;
			// }

			// get values of the left and right sticks
			leftStickYVal = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
			rightStickYVal = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_Y);

			// Puts both stick values through sensitivity curve function
			leftVal = sensitivityCurve(leftStickYVal);
			rightVal = sensitivityCurve(rightStickYVal);

			// Sets motors to values
			if (master.get_digital(pros::E_CONTROLLER_DIGITAL_DOWN))
			{
				// Ryans idea, when DOWN is pressed do a slow straight backup
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

			// Sets motor speeds based on R! and R2
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
		}
		pros::delay(20);
	}
}

void opcontrol()
{
	pros::Task liftTrayConrtol_task(liftTrayControl, 0, TASK_PRIORITY_DEFAULT);

	pros::Task driveIntakeControl_task(driveIntakeControl, 0, TASK_PRIORITY_DEFAULT);
}
