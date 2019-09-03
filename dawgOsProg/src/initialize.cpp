
#include "main.h"
#include "globals.hpp"

const char * autonTypes[] = {"1", "2", "3", "4", ""};

#include "interface.hpp"


pros::Controller master(pros::E_CONTROLLER_MASTER);

int driveFR_port = 1;
int driveFL_port = 2;
int driveBR_port = 3;
int driveBL_port = 4;
int tray_port    = 5;
int lift_port    = 6;
int intakeR_port = 7;
int intakeL_port = 8;

// PORT 7 DEAD ON TEST BRAIN
pros::Motor driveFR (driveFR_port, MOTOR_GEARSET_18, true,  MOTOR_ENCODER_DEGREES);
pros::Motor driveFL (driveFL_port, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Motor driveBR (driveBR_port, MOTOR_GEARSET_18, true,  MOTOR_ENCODER_DEGREES);
pros::Motor driveBL (driveBL_port, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Motor tray    (tray_port,    MOTOR_GEARSET_36, false, MOTOR_ENCODER_DEGREES);
pros::Motor lift    (lift_port,    MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Motor intakeR (intakeR_port, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Motor intakeL (intakeL_port, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);

int autonColor = 1;
int autonType = 0;


std::string debugOutput = "";
bool autonArmed = false;
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	// Starts the interface as a task
	pros::Task interface_task(interfaceInit, 0);
	// methodTester();
	// interfaceInit();
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