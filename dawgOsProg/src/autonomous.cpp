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
    trayControl.setMaxVelocity(60);
    liftControl.setMaxVelocity(200);
    trayControl.setTarget(240);
    liftControl.setTarget(910);
    while (!liftControl.isSettled())
    {
        pros::delay(20);
    }
}

void dropStack()
{
    trayControl.setMaxVelocity(100);
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

void autonomous()
{
    intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
	intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
    driveControl.setMaxVelocity(50);
    if (autonType == 0)
    {
        // ONE CUBE SPIT

        intakeOut();
        driveControl.moveDistance(0.3_ft);
        pros::delay(1000);
        intakeStop();
    }
    else if (autonType == 1)
    {
        // FOUR CUBE STACK IN UNPROTECTED ZONE

        trayControl.setMaxVelocity(40);
        liftControl.setMaxVelocity(200);
        // Rolls out to dispense tray
        intakeOut();
        pros::delay(500);
        // Pick up 4 cubes
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
    else if (autonType == 3)
    {
        // SKILLS AUTON

        trayControl.setMaxVelocity(40);
        liftControl.setMaxVelocity(200);
        // Rolls out to dispense tray
        intakeOut();
        pros::delay(500);
        // Pick up 4 cubes
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
        driveControl.turnAngle(-135_deg);
        // Pick up cube at base
        intakeIn();
        driveControl.moveDistance(1.4_ft);
        driveControl.moveDistance(-0.3_ft);
        intakeOut();
        pros::delay(400);
        intakeStop();
        liftUp();
        driveControl.moveDistance(0.15_ft);
        intakeSpit();
        driveControl.moveDistance(-0.5_ft);
        liftControl.stop();
        trayControl.stop();
    }
    else if (autonType == 2)
    {
        dropStack();
    }

    return;
}
