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
pros::ADIButton traySense('H');

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
	if(input >= 450){
		output = -1 * (0.000178571428571 * (input - 700) * (input - 700)) + 40;
	}
	else{
		output = (0.000178571428571 * (input - 700) * (input - 700)) + 40;
	}
	// All that work just to eventually make it quadratic lol
	debugOutput = std::to_string(output);
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

void liftTrayContol(void *param)
{
	int liftVal;
	int trayVal;
	int trayMacroPos = 316;
	int liftMidPos = 405;
	int liftHighPos = 620;
	int liftMostlyDownPos = 120;
	liftTrayEnabled = true;

	intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
	intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
	lift.set_brake_mode(MOTOR_BRAKE_HOLD);
	tray.set_brake_mode(MOTOR_BRAKE_HOLD);

	while (true)
	{
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
				trayVal = 75;
				autoLED = 2;
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L2))
			{
				trayVal = -90;
				autoLED = 2;
			}
			else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_L1) && tray.get_position() < 650)
			{
				trayVal = trayCurveV3((float)tray.get_position());
				liftVal = -30;
				autoLED = 3;
			}
			else
			{
				trayVal = 0;
				autoLED = 2;
			}
			while (liftTrayEnabled)
			{
				if (master.get_digital(pros::E_CONTROLLER_DIGITAL_X) && lift.get_position() < liftHighPos)
				{
					// Lift HIGH Macro
					autoLED = 3;
					if (tray.get_position() < trayMacroPos)
					{
						tray = 127;
						lift = 100;
						while (tray.get_position() < trayMacroPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						tray = 0;
					}
					else if (tray.get_position() > trayMacroPos)
					{
						tray = -127;
						while (tray.get_position() > trayMacroPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						tray = 0;
						lift = 100;
					}
					while (lift.get_position() < liftHighPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					lift = 0;
					autoLED = 2;
				}
				else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && lift.get_position() < liftMidPos)
				{
					// Lift MID Macro for when lift is BELOW POSITION
					autoLED = 3;
					if (tray.get_position() < trayMacroPos)
					{
						tray = 127;
						lift = 100;
						while (tray.get_position() < trayMacroPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						tray = 0;
					}
					else if (tray.get_position() > trayMacroPos)
					{
						tray = -127;
						while (tray.get_position() > trayMacroPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						tray = 0;
						lift = 100;
					}
					while (lift.get_position() < liftMidPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					lift = 0;
					autoLED = 2;
				}
				else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_Y) && lift.get_position() > liftMidPos)
				{
					// Lift MID Macro for when lift is ABOVE POSITION
					autoLED = 3;
					if (tray.get_position() < trayMacroPos)
					{
						tray = 127;
						while (tray.get_position() < trayMacroPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						tray = 0;
						lift = -100;
					}
					else if (tray.get_position() > trayMacroPos)
					{
						tray = -127;
						while (tray.get_position() > trayMacroPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
						{
							pros::delay(20);
						}
						tray = 0;
						lift = -100;
					}
					while (lift.get_position() > liftMidPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					lift = 0;
					autoLED = 2;
				}
				else if (master.get_digital(pros::E_CONTROLLER_DIGITAL_B) && lift.get_position() > 0)
				{
					// Lift DOWN Macro
					if (tray.get_position() > trayMacroPos + 30)
					{
						tray = -127;
					}
					else if (tray.get_position() < trayMacroPos - 30)
					{
						tray = 127;
					}
					while ((tray.get_position() < trayMacroPos - 30 || tray.get_position() > trayMacroPos + 30) && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					tray = 0;
					autoLED = 3;
					lift = -100;
					while (lift.get_position() > liftMostlyDownPos && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					lift.set_brake_mode(MOTOR_BRAKE_COAST);
					lift = -40;
					tray = -127;
					while (tray.get_position() > 15 && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					tray = 0;
					tray.set_zero_position(0);
					while (lift.get_position() > 15 && !master.get_digital(pros::E_CONTROLLER_DIGITAL_A))
					{
						pros::delay(20);
					}
					lift = 0;
					lift.set_brake_mode(MOTOR_BRAKE_HOLD);
					autoLED = 2;
				}
				else
				{
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

void opcontrol()
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
	int backupVal = -23;

	pros::Task liftTrayContol_task(liftTrayContol, 0, TASK_PRIORITY_DEFAULT);

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
