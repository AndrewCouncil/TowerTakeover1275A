#include "main.h"
#include "globals.hpp"
/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */

// struct cartVect{
//     float x, y;
// }

// struct polVect{
//     float l, theta;
// }
using namespace okapi;

auto myChassis = ChassisControllerFactory::create(
  {(int8_t) driveFL_port, (int8_t) (-driveFR_port)}, {(int8_t) driveFL_port, (int8_t) (-driveBR_port)},
  AbstractMotor::gearset::green,
  {4_in, 11.2_in}
);

void autonomous() {
    driveFR = 125;
    pros::delay(1000);
    driveFR = 0;
    return;
}
