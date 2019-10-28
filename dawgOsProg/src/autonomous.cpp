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

auto driveControl = ChassisControllerFactory::create(
    {(int8_t)driveFL_port, (int8_t)driveBL_port}, {(int8_t)(-driveFR_port), (int8_t)(-driveBR_port)},
    AbstractMotor::gearset::green,
    {4_in, 11_in});

auto trayControl = AsyncControllerFactory::posIntegrated((int8_t)(-tray_port));
auto liftControl = AsyncControllerFactory::posIntegrated((int8_t)lift_port);
/*
void rollout() {
    trayControl.setMaxVelocity(150);
    liftControl.setMaxVelocity(300);
    trayControl.setTarget(340);
    liftControl.setTarget(300);
    while (!trayControl.isSettled()) {pros::delay(20);}
    liftControl.setTarget(-175);
    // while (!liftControl.isSettled()) {pros::delay(20);}
    trayControl.setTarget(-60);
    return;
}*/

void intakeIn()
{
    intakeL = 127;
    intakeR = 127;
    return;
}

void intakeOut()
{
    intakeL = -127;
    intakeR = -127;
    return;
}

void intakeStop()
{
    intakeL = 0;
    intakeR = 0;
    return;
}

// Spits out cube from intake
void intakeSpit()
{
    intakeL = -88;
    intakeR = -88;
    pros::delay(700);
    intakeL = 0;
    intakeR = 0;
}

void liftUp()
{
    intakeOut();
    pros::delay(400);
    intakeStop();
    
    trayControl.setMaxVelocity(60);
    liftControl.setMaxVelocity(200);
    trayControl.setTarget(240);
    liftControl.setTarget(910);
    while (!liftControl.isSettled())
    {
        pros::delay(20);
    }
}

void liftDown(bool wait)
{
    trayControl.setMaxVelocity(60);
    liftControl.setMaxVelocity(200);
    trayControl.setTarget(0);
    liftControl.setTarget(0);
    while (!liftControl.isSettled() && wait)
    {
        pros::delay(20);
    }
}

void dropStack()
{
    trayControl.setMaxVelocity(70);
    // intakeL = -30;
    // intakeR = -30;
    trayControl.setTarget(800);
    while (!trayControl.isSettled())
    {
        pros::delay(20);
    }
    pros::delay(1000);
    driveControl.setMaxVelocity(25);
    driveControl.moveDistance(-0.4_ft);
    driveControl.setMaxVelocity(50);
    intakeStop();
    trayControl.setTarget(0);
    return;
}

void fourPoint(){
    // FOUR CUBE STACK IN UNPROTECTED ZONE
    driveControl.setMaxVelocity(50);

    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);
    // Rolls out to dispense tray
    intakeOut();
    pros::delay(1000);
    // Pick up 4 cubes
    trayControl.setTarget(0);
    intakeIn();
    driveControl.moveDistance(3_ft);
    intakeStop();
    // Back up and turn toward corner
    driveControl.moveDistance(-1.7_ft);
    driveControl.turnAngle(135_deg * autonColor);
    driveControl.waitUntilSettled();
    driveControl.stop();
    // Drive to goal and place
    driveControl.moveDistance(1.15_ft);
    dropStack();
}

void oldSkills(){
    // SKILLS AUTON
    driveControl.setMaxVelocity(50);

    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);
    // Rolls out to dispense tray
    intakeOut();
    pros::delay(500);
    // Pick up 4 cubes
    trayControl.setTarget(0);
    intakeIn();
    driveControl.moveDistance(3_ft);
    intakeStop();
    // Go around pole and lign up
    driveControl.turnAngle(45_deg);
    driveControl.moveDistance(1_ft);
    driveControl.turnAngle(-45_deg);
    driveControl.moveDistance(2.25_ft);
    driveControl.turnAngle(-45_deg);
    driveControl.moveDistance(0.85_ft);
    driveControl.turnAngle(45_deg);
    // Pick up 3 cubes
    intakeIn();
    driveControl.moveDistance(2.2_ft);
    pros::delay(1000);
    intakeStop();
    // Place down stack
    driveControl.turnAngle(45_deg);
    driveControl.moveDistance(1.55_ft);
    dropStack();
    // Line up with tower
    driveControl.moveDistance(-1.7_ft);
    driveControl.turnAngle(135_deg);
    // Pick up cube at base
    intakeIn();
    driveControl.moveDistance(1.4_ft);
    driveControl.moveDistance(-0.3_ft);
    intakeOut();
    pros::delay(400);
    intakeStop();
    liftUp();
    intakeSpit();
    driveControl.moveDistance(-0.5_ft);
    liftControl.stop();
    trayControl.stop();
}

void absurdSkills(){
    // Theoretical 56 point skills autonomous

    // Realistically, this probably won't work, but on the off chance, I need to crush everyone
    
    driveControl.setMaxVelocity(75);

    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);

    // Rolls out to dispense tray
    intakeOut();
    pros::delay(1000);

    // Pick up 3 cubes
    trayControl.setTarget(0);
    intakeIn();
    driveControl.moveDistance(3_ft);
    intakeStop();

    // Move back and lign up with 4 cube row
    driveControl.moveDistance(-2.5_ft);
    driveControl.turnAngle(90_deg);
    driveControl.moveDistance(1.7_ft);
    driveControl.turnAngle(-90_deg);

    // Pick up 4 cubes
    intakeIn();
    driveControl.moveDistance(2.5_ft);
    intakeStop();

    // Back up and turn toward corner
    driveControl.moveDistance(-1.7_ft);
    driveControl.turnAngle(135_deg);
    driveControl.waitUntilSettled();
    driveControl.stop();

    // Drive to goal and place
    driveControl.moveDistance(1.15_ft);
    dropStack();

    return;

    // Back up and lign up for cross drive
    driveControl.moveDistance(-0.5_ft);
    driveControl.turnAngle(45_deg);
    driveControl.moveDistance(-2.3_ft);
    driveControl.turnAngle(90_deg);
    
    //Lign up on wall
    driveControl.moveDistance(-1_ft);
    driveControl.moveDistance(1_ft);

    // Drive across and pick up 3 cubes
    intakeIn();
    driveControl.moveDistance(6.5_ft);
    intakeStop();

    // Lign up with 4 cube row
    driveControl.turnAngle(90_deg);
    driveControl.moveDistance(2.3_ft);
    driveControl.turnAngle(90_deg);

    // Pick up 4 cubes
    intakeIn();
    driveControl.moveDistance(2.5_ft);
    intakeStop();

    // Back up and turn toward corner
    driveControl.moveDistance(-1.7_ft);
    driveControl.turnAngle(-135_deg);
    driveControl.waitUntilSettled();
    driveControl.stop();

    // Drive to goal and place
    driveControl.moveDistance(1.15_ft);
    dropStack();

    // Lign up with short middle tower
    driveControl.moveDistance(-1.3_ft);
    driveControl.turnAngle(135_deg);

    // Pick up cube at base
    intakeIn();
    driveControl.moveDistance(2_ft);
    intakeStop();

    // Back up and lift
    driveControl.moveDistance(-0.3_ft);
    liftUp();

    // Move up(?) and place
    intakeSpit();

    // Move back(?) and lift down
    liftDown(true);

    // Pick up cube to right of tower
    intakeIn();
    driveControl.turnAngle(-30_deg);
    driveControl.moveDistance(0.3_ft);

    // Move back out
    driveControl.moveDistance(-0.3_ft);
    driveControl.turnAngle(30_deg);

    // Lign up with blue tower
    driveControl.moveDistance(-2_ft);
    driveControl.turnAngle(-90_deg);
    
    // Lift up and move up to tower and place
    liftUp();
    driveControl.moveDistance(1.3_ft);
    intakeSpit();

    // Move back and lift down
    liftDown(false);
    driveControl.moveDistance(-1_ft);

    // Lign up with blue side tallish tower
    driveControl.turnAngle(90_deg);
    driveControl.moveDistance(.5_ft);
    driveControl.turnAngle(90_deg);

    // Pick up cube at tower base
    intakeIn();
    driveControl.moveDistance(2_ft);
    intakeStop();

    // Back up and lift
    driveControl.moveDistance(-0.3_ft);
    liftUp();

    // Move up(?) and place
    intakeSpit();
}

void oneCube(){
    // ONE CUBE SPIT

    intakeOut();
    driveControl.moveDistance(0.3_ft);
    pros::delay(1000);
    intakeStop();
}

void autonomous()
{
    intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
	intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
    if (autonType == 0)
    {
        oneCube();
    }
    else if (autonType == 1)
    {
        fourPoint();
    }
    else if (autonType == 3)
    {
        oldSkills();
    }
    else if (autonType == 2)
    {
        dropStack();
    }

    return;
}
