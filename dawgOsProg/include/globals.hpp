#pragma once
#include "okapi/api.hpp"
#include "pros/apix.h"
#include "api.h"
#include <string>

extern pros::Controller master;

extern int driveFR_port;
extern int driveFL_port;
extern int driveBR_port;
extern int driveBL_port;
extern int tray_port;
extern int lift_port;
extern int intakeR_port;
extern int intakeL_port;

extern pros::Motor driveFR;
extern pros::Motor driveFL;
extern pros::Motor driveBR;
extern pros::Motor driveBL;
extern pros::Motor tray;
extern pros::Motor lift;
extern pros::Motor intakeR;
extern pros::Motor intakeL;
extern pros::ADIPort led;
extern pros::ADIDigitalIn traySense;


extern int autonColor;
extern int autonType;
extern bool autonArmed;
extern const char * autonTypes[5];

extern int autoLED;
extern int selectedLED;

extern float robotX;
extern float robotY;
extern const long pi;
extern const long radConv;

extern std::string debugOutput;