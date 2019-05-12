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
extern int driveM_port;
extern int liftR_port;
extern int liftL_port;
extern int intake_port;

extern pros::Motor driveFR;
extern pros::Motor driveFL;
extern pros::Motor driveBR;
extern pros::Motor driveBL;
extern pros::Motor driveM;
extern pros::Motor liftR;
extern pros::Motor liftL;
extern pros::Motor intake;


extern int autonColor;
extern int autonType;
extern bool autonArmed;
extern const char * autonTypes[5];

extern std::string debugOutput;