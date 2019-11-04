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
    {4_in, 11.9_in});

auto profileController = AsyncControllerFactory::motionProfile(
  1.0,  // Maximum linear velocity of the Chassis in m/s
  2.0,  // Maximum linear acceleration of the Chassis in m/s/s
  10.0, // Maximum linear jerk of the Chassis in m/s/s/s
  driveControl // Chassis Controller
);

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

void liftUp(bool high)
{
    int liftTarget;
    intakeOut();
    pros::delay(250);
    intakeStop();
    if(high){
        liftTarget = 1000;
    }
    else{
        liftTarget = 765;
    }

    trayControl.setMaxVelocity(150);
    liftControl.setMaxVelocity(200);
    trayControl.setTarget(480);
    liftControl.setTarget(liftTarget);
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
    pros::delay(1000);
    liftControl.setTarget(0);
    while (!liftControl.isSettled() && wait)
    {
        pros::delay(20);
    }
}

void dropStack()
{
    trayControl.setMaxVelocity(55);
    // intakeL = -30;
    // intakeR = -30;
    trayControl.setTarget(800);
    while (!trayControl.isSettled())
    {
        pros::delay(20);
    }
    pros::delay(500);
    driveControl.setMaxVelocity(30);
    driveControl.moveDistance(-0.4_ft);
    driveControl.setMaxVelocity(50);
    intakeStop();
    trayControl.setTarget(0);
    driveControl.setMaxVelocity(85);
    return;
}

void dropStackAsync(){
    trayControl.setMaxVelocity(55);
    // intakeL = -30;
    // intakeR = -30;
    trayControl.setTarget(800);
}

void fourStack(){
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
    driveControl.moveDistance(-1.3_ft);
    driveControl.turnAngle(135_deg * autonColor);
    driveControl.waitUntilSettled();
    driveControl.stop();
    // Drive to goal and place
    driveControl.moveDistance(1.5_ft);
    dropStack();
    return;
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
    liftUp(false);
    intakeSpit();
    driveControl.moveDistance(-0.5_ft);
    liftControl.stop();
    trayControl.stop();
}

void absurdSkills(){
    // Theoretical 56 point skills autonomous

    // Realistically, this probably won't work, but on the off chance, I need to crush everyone
    
    driveControl.setMaxVelocity(70);

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
    driveControl.moveDistance(2_ft);
    driveControl.turnAngle(-90_deg);
    // Pick up 4 cubes
    intakeIn();
    driveControl.moveDistance(2.8_ft);

    // Back up and turn toward corner
    driveControl.moveDistance(-1.9_ft);
    driveControl.turnAngle(135_deg);
    driveControl.waitUntilSettled();
    driveControl.stop();

    // Drive to goal and place
    driveControl.moveDistance(1.15_ft);
    intakeStop();
    dropStack();

    // Back up and lign up to tower
    driveControl.setMaxVelocity(50);
    driveControl.moveDistance(-1.6_ft);
    driveControl.turnAngle(-135_deg);

    // Pick up cube at base
    intakeIn();
    driveControl.moveDistance(2.15_ft);
    intakeStop();

    // Back up and lift and place
    driveControl.moveDistance(-0.7_ft);
    liftUp(false);
    driveControl.setMaxVelocity(20);
    driveControl.moveDistance(0.45_ft);
    intakeSpit();
    return;
    // Move down and lign up with wall
    liftDown(false);
    driveControl.moveDistance(-4.3_ft);

    // Lign up with tower
    driveControl.moveDistance(1.5_ft);
    driveControl.turnAngle(-90_deg);

    // Pick up cube at base
    intakeIn();
    driveControl.moveDistance(2.5_ft);
    intakeStop();

    // Back up and lift and place
    driveControl.moveDistance(-0.5_ft);
    liftUp(true);
    intakeSpit();
    return;

    // Move down lift and move back and lign up with cube to left
    liftDown(false);
    driveControl.moveDistance(-0.3_ft);
    driveControl.turnAngle(-23_deg);
}

void eightStack(){
    // Yeet 8 cubes into the zone, very fast boi

    driveControl.setMaxVelocity(70);

    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);

    // Rolls out to dispense tray and lign up with wall(?)
    intakeOut();
    pros::delay(500);
    driveControl.setMaxVelocity(40);
    driveControl.moveDistanceAsync(-0.5_ft);
    trayControl.setTarget(0);
    pros::delay(500);
    driveControl.stop();

    // Pick up 3 cubes
    intakeIn();
    driveControl.setMaxVelocity(85);
    driveControl.moveDistance(3_ft);
    intakeStop();

    // Turn to cross and cross and lign up
    driveControl.setMaxVelocity(70);
    driveControl.turnAngle(-20_deg * autonColor);
    driveControl.setMaxVelocity(85);
    driveControl.moveDistance(-3.6_ft);
    driveControl.setMaxVelocity(70);
    driveControl.turnAngle(20_deg * autonColor);

    // Pick up 4 cubes
    trayControl.setTarget(0);
    intakeIn();
    driveControl.setMaxVelocity(85);
    driveControl.moveDistance(2.5_ft);
    intakeStop();
    // Back up and turn toward corner
    driveControl.moveDistance(-1.3_ft);
    driveControl.setMaxVelocity(70);
    driveControl.turnAngle(135_deg * autonColor);
    
    driveControl.waitUntilSettled();
    driveControl.stop();
    // Drive to goal and place
    driveControl.setMaxVelocity(85);
    driveControl.moveDistance(1.5_ft);
    dropStack();
    return;
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
    pros::delay(30);
    if (autonType == 0)
    {
        fourStack();
        // oneCube();
    }
    else if (autonType == 1)
    {
        oneCube();
    }
    else if (autonType == 3)
    {
        absurdSkills();
    }
    else if (autonType == 2)
    {
        oneCube();
    }

    return;
}