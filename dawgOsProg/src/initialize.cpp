
#include "main.h"
#include "globals.hpp"

const char *autonTypes[] = {"1", "2", "3", "4", ""};

#include "interface.hpp"

pros::Controller master(pros::E_CONTROLLER_MASTER);

int driveFR_port = 1;
int driveFL_port = 2;
int driveBR_port = 3;
int driveBL_port = 4;
int tray_port = 5;
int lift_port = 6;
int intakeR_port = 7;
int intakeL_port = 9;

// PORT 7 DEAD ON TEST BRAIN
pros::Motor driveFR(driveFR_port, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);
pros::Motor driveFL(driveFL_port, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Motor driveBR(driveBR_port, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);
pros::Motor driveBL(driveBL_port, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Motor tray(tray_port, MOTOR_GEARSET_36, false, MOTOR_ENCODER_DEGREES);
pros::Motor lift(lift_port, MOTOR_GEARSET_36, false, MOTOR_ENCODER_DEGREES);
pros::Motor intakeR(intakeR_port, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);
pros::Motor intakeL(intakeL_port, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);

pros::ADIMotor led('C');
// pros::ADIButton traySense(6);

int autonColor = 1;
int autonType = 0;

int autoLED = 2;
int selectedLED = 0;
// 0 is go to automatic selection

// FOR BOTH:
// 1 is OFF
// 2 is LOW
// 3 is HIGH
// 4 is FADE
// 5 is PULSE
// 6 is STROBE

std::string debugOutput = "";
bool autonArmed = false;

void LEDControl(void* param)
{
	int modeLED;
	int fadeCount = 0;
	int pulseCount = 0;
	int strobeCount = 0;
	while (true)
	{
		// Set LED mode to automatic selection or the selected one
		// if (selectedLED == 0)
		// {
		// 	modeLED = autoLED;
		// }
		// else
		// {
		// 	modeLED = selectedLED;
		// }

		// switch (modeLED)
		// {

		// case 1:
		// 	led.set_value(0);

		// case 2:
		// 	led.set_value(74);

		// case 3:
		// 	led.set_value(127);

		// case 4:
		// 	if (fadeCount <= 127)
		// 	{
		// 		led.set_value(fadeCount);
		// 	}
		// 	else
		// 	{
		// 		led.set_value(254 - fadeCount);
		// 	}
		// 	fadeCount++;
		// 	if (fadeCount > 254)
		// 	{
		// 		fadeCount = 0;
		// 	}

		// case 5:
		// 	if (pulseCount <= 64)
		// 	{
		// 		led.set_value(63 + pulseCount);
		// 	}
		// 	else
		// 	{
		// 		led.set_value(191 - pulseCount);
		// 	}
		// 	pulseCount++;
		// 	if (pulseCount > 127)
		// 	{
		// 		pulseCount = 0;
		// 	}
		// case 6:
		// 	if (strobeCount <= 12)
		// 	{
		// 		led.set_value(127);
		// 	}
		// 	else
		// 	{
		// 		led.set_value(0);
		// 	}
		// 	strobeCount++;
		// 	if (strobeCount > 24)
		// 	{
		// 		strobeCount = 0;
		// 	}

		// default:
		// 	led.set_value(0);
		// }
		debugOutput = "LED WORKING";
		// led(127);
		pros::delay(30);
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize()
{
	// Starts the interface as a task
	pros::Task interface_task(interfaceInit, 0, TASK_PRIORITY_DEFAULT);
	// pros::Task led_task      (LEDControl,    0, TASK_PRIORITY_DEFAULT);
}
//
/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}