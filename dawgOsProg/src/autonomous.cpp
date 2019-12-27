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
    {(int8_t)(driveFL_port), (int8_t)(driveBL_port)}, {(int8_t)(driveFR_port), (int8_t)(driveBR_port)},
    AbstractMotor::gearset::green,
    {4_in, 9.6_in});

auto profileController = AsyncControllerFactory::motionProfile(
    1.0,         // Maximum linear velocity of the Chassis in m/s
    2.0,         // Maximum linear acceleration of the Chassis in m/s/s
    10.0,        // Maximum linear jerk of the Chassis in m/s/s/s
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

int trayUpVal = 663;

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
    return;
}

void liftUp(bool high)
{
    int liftTarget;
    intakeOut();
    pros::delay(250);
    intakeStop();
    if (high)
    {
        liftTarget = 620;
    }
    else
    {
        liftTarget = 415;
    }

    trayControl.setMaxVelocity(150);
    liftControl.setMaxVelocity(200);
    trayControl.setTarget(336);
    liftControl.setTarget(liftTarget);
    while (!liftControl.isSettled())
    {
        pros::delay(20);
    }
}

void liftDown(bool wait)
{
    trayControl.setMaxVelocity(50);
    liftControl.setMaxVelocity(200);
    liftControl.setTarget(0);
    pros::delay(750);
    trayControl.setTarget(0);
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
    liftControl.setTarget(-10);
    trayControl.setTarget(trayUpVal);
    while (!trayControl.isSettled())
    {
        pros::delay(20);
    }
    pros::delay(500);
    driveControl.setMaxVelocity(30);
    liftControl.stop();
    driveControl.moveDistance(-0.4_ft);
    driveControl.setMaxVelocity(50);
    intakeStop();
    // trayControl.setTarget(0);
    // driveControl.setMaxVelocity(85);
    return;
}

void dropStackAsync()
{
    trayControl.setMaxVelocity(55);
    liftControl.setTarget(-10);
    // intakeL = -30;
    // intakeR = -30;
    trayControl.setTarget(trayUpVal);
}

void foldOut(bool wait)
{
    driveControl.setMaxVelocity(70);
    trayControl.setMaxVelocity(55);
    liftControl.setMaxVelocity(200);
    // driveControl.moveDistance(0.3_ft);
    // driveControl.moveDistanceAsync(-0.3_ft);
    pros::delay(30);
    trayControl.setTarget(220);
    liftControl.setTarget(150);
    intakeOut();
    // driveControl.waitUntilSettled();
    while(!liftControl.isSettled()){
        intakeOut();
        pros::delay(20);
    }
    trayControl.setTarget(0);
    liftControl.setTarget(30);
    while(!liftControl.isSettled() && wait){
        pros::delay(20);
    }
    if(wait){
        lift.set_zero_position(0);
    }
    return;
}

void fiveStack()
{
    driveControl.setMaxVelocity(65);
    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);

    foldOut(false);
    // Pick up 4 cubes
    intakeIn();
    driveControl.moveDistance(3_ft);
    liftControl.stop();
    lift.set_zero_position(0);
    // Back up and turn toward corner
    driveControl.moveDistance(-1.3_ft);
    intakeStop();
    driveControl.turnAngle(135_deg * autonColor);
    driveControl.waitUntilSettled();
    driveControl.stop();
    // Drive to goal and place
    driveControl.moveDistance(1.37_ft);
    dropStack();
    return;
}

void oldFiveStack()
{
    // five CUBE STACK IN UNPROTECTED ZONE
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
    // Back up and turn toward corner
    driveControl.moveDistance(-1.3_ft);
    intakeStop();
    driveControl.turnAngle(135_deg * autonColor);
    driveControl.waitUntilSettled();
    driveControl.stop();
    // Drive to goal and place
    driveControl.moveDistance(1.37_ft);
    dropStack();
    return;
}

void sixStraight()
{
    // Six cube stack that goes straight for purple
    driveControl.setMaxVelocity(63);

    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);

    foldOut(false);

    // Pick up 5 cubes
    trayControl.setTarget(0);
    intakeIn();
    driveControl.moveDistance(4.6_ft);
    // driveControl.turnAngle(-7_deg * autonColor);
    pros::delay(500);
    // driveControl.turnAngle(7_deg * autonColor);

    // Back up and turn toward corner
    driveControl.moveDistance(-2_ft);
    driveControl.turnAngle(135_deg * autonColor);
    driveControl.waitUntilSettled();
    driveControl.stop();

    // Drive to goal and place
    intakeOut();
    pros::delay(400);
    intakeStop();
    dropStackAsync();
    driveControl.setMaxVelocity(45);
    driveControl.moveDistance(1.35_ft);
    while (!trayControl.isSettled())
    {
        pros::delay(20);
    }
    driveControl.setMaxVelocity(30);
    driveControl.moveDistance(-0.4_ft);
    driveControl.setMaxVelocity(50);
    intakeStop();
    return;
}

void sixBent()
{
    // Six stack that turns for orange/green
    driveControl.setMaxVelocity(65);
    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);

    foldOut(false);
    // Pick up 5 cubes
    trayControl.setTarget(0);
    intakeIn();
    driveControl.moveDistance(3_ft);
    // Pick up 6th cube
    driveControl.turnAngle(-45_deg * autonColor);
    driveControl.moveDistance(1_ft);
    driveControl.moveDistance(-1_ft);
    driveControl.turnAngle(45_deg * autonColor);
    // Back up and turn toward corner
    driveControl.moveDistance(-1.3_ft);
    intakeStop();
    driveControl.turnAngle(135_deg * autonColor);
    driveControl.waitUntilSettled();
    driveControl.stop();
    // Drive to goal and place

    // Drive to goal and place
    intakeStop();
    dropStackAsync();
    driveControl.setMaxVelocity(45);
    driveControl.moveDistance(1.35_ft);
    while (!trayControl.isSettled())
    {
        pros::delay(20);
    }
    driveControl.setMaxVelocity(30);
    driveControl.moveDistance(-0.4_ft);
    driveControl.setMaxVelocity(50);
    intakeStop();
    return;
}

void sixProtected(){
    driveControl.setMaxVelocity(65);
    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);
    foldOut(false);
    pros::delay(500);
    lift.set_zero_position(0);

    liftUp(true);
    driveControl.moveDistance(2.4_ft);
    driveControl.turnAngle(27_deg * autonColor);
    driveControl.setMaxVelocity(45);
    driveControl.moveDistance(0.6_ft);
    intakeIn();
    liftDown(true);
    driveControl.moveDistance(0.5_ft);
    driveControl.setMaxVelocity(65);
    driveControl.turnAngle(153_deg * autonColor);
    driveControl.moveDistance(2_ft);
    driveControl.turnAngle(65_deg * autonColor);
    
    // Drive to goal and place
    intakeStop();
    driveControl.moveDistanceAsync(2_ft);
    pros::delay(1300);
    dropStackAsync();
    
    while (!trayControl.isSettled())
    {
        pros::delay(20);
    }
    driveControl.setMaxVelocity(30);
    driveControl.moveDistance(-0.4_ft);
    driveControl.setMaxVelocity(50);
    intakeStop();
    return;
}

void oldSixStack()
{
    // five CUBE STACK IN UNPROTECTED ZONE
    driveControl.setMaxVelocity(63);

    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);
    // Rolls out to dispense tray
    intakeOut();
    intakeOut();
    pros::delay(400);
    // Pick up 5 cubes
    trayControl.setTarget(0);
    intakeIn();
    driveControl.moveDistance(4.6_ft);
    // driveControl.turnAngle(-7_deg * autonColor);
    pros::delay(500);
    // driveControl.turnAngle(7_deg * autonColor);

    // Back up and turn toward corner
    driveControl.moveDistance(-3.1_ft);
    driveControl.turnAngle(135_deg * autonColor);
    driveControl.waitUntilSettled();
    driveControl.stop();

    // Drive to goal and place
    intakeStop();
    dropStackAsync();
    driveControl.setMaxVelocity(45);
    driveControl.moveDistance(1.35_ft);
    while (!trayControl.isSettled())
    {
        pros::delay(20);
    }
    driveControl.setMaxVelocity(30);
    driveControl.moveDistance(-0.4_ft);
    driveControl.setMaxVelocity(50);
    intakeStop();
    return;
}
void eightStack()
{
    // Yeet 8 cubes into the zone, very fast boi

    driveControl.setMaxVelocity(70);

    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);

    // Rolls out to dispense tray and lign up with wall(?)
    intakeOut();
    intakeOut();
    pros::delay(750);
    // driveControl.setMaxVelocity(40);
    // driveControl.moveDistanceAsync(-0.5_ft);
    trayControl.setTarget(0);
    // pros::delay(500);
    // driveControl.stop();

    // Pick up 3 cubes
    intakeIn();
    driveControl.setMaxVelocity(90);
    driveControl.moveDistance(3_ft);
    intakeStop();

    // Turn to cross and cross and lign up
    driveControl.setMaxVelocity(70);
    driveControl.turnAngle(-37_deg * autonColor);
    driveControl.setMaxVelocity(100);
    driveControl.moveDistance(-3.12_ft);
    driveControl.setMaxVelocity(70);
    driveControl.turnAngle(40_deg * autonColor);

    // Pick up 4 cubes
    trayControl.setTarget(0);
    intakeIn();
    driveControl.setMaxVelocity(60);
    driveControl.moveDistance(3.2_ft);
    // Back up and turn toward corner
    driveControl.setMaxVelocity(100);
    driveControl.moveDistance(-2.2_ft);
    driveControl.setMaxVelocity(70);
    driveControl.turnAngle(135_deg * autonColor);
    driveControl.waitUntilSettled();
    driveControl.stop();

    // Drive to goal and place
    intakeStop();
    dropStackAsync();
    driveControl.setMaxVelocity(30);
    driveControl.moveDistance(1.5_ft);
    while (!trayControl.isSettled())
    {
        pros::delay(20);
    }
    driveControl.setMaxVelocity(30);
    driveControl.moveDistance(-0.4_ft);
    driveControl.setMaxVelocity(50);
    intakeStop();
    return;
}

void eightStackPathing()
{
    if (autonColor == 1)
    {
        profileController.generatePath({Point{0_ft, 0_ft, 0_deg}, Point{2_ft, -5_ft, -64_deg}, Point{4_ft, -10_ft, 0_deg}}, "A");
    }
    else
    {
        profileController.generatePath({Point{0_ft, 0_ft, 0_deg},
                                        Point{3_ft, 12_ft, 0_deg}},
                                       "A");
    }
    // Yeet 8 cubes into the zone, very fast boi

    driveControl.setMaxVelocity(70);

    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);

    // Rolls out to dispense tray and lign up with wall(?)
    intakeOut();
    intakeOut();
    pros::delay(750);
    // driveControl.setMaxVelocity(40);
    // driveControl.moveDistanceAsync(-0.5_ft);
    trayControl.setTarget(0);
    // pros::delay(500);
    // driveControl.stop();

    // Pick up 3 cubes
    intakeIn();
    driveControl.setMaxVelocity(80);
    driveControl.moveDistance(3_ft);
    intakeStop();

    // Lign up with the row of 4
    driveFR.set_reversed(false);
    driveBR.set_reversed(false);
    driveFL.set_reversed(true);
    driveBL.set_reversed(true);
    profileController.setTarget("A");
    profileController.waitUntilSettled();
    driveFR.set_reversed(true);
    driveBR.set_reversed(true);
    driveFL.set_reversed(false);
    driveBL.set_reversed(false);
    return;

    // Pick up 4 cubes
    trayControl.setTarget(0);
    intakeIn();
    driveControl.setMaxVelocity(65);
    driveControl.moveDistance(3.2_ft);
    // Back up and turn toward corner
    driveControl.setMaxVelocity(80);
    driveControl.moveDistance(-2.2_ft);
    driveControl.setMaxVelocity(70);
    driveControl.turnAngle(135_deg * autonColor);
    driveControl.waitUntilSettled();
    driveControl.stop();

    // Drive to goal and place
    intakeStop();
    dropStackAsync();
    driveControl.setMaxVelocity(30);
    driveControl.moveDistance(1.5_ft);
    while (!trayControl.isSettled())
    {
        pros::delay(20);
    }
    driveControl.setMaxVelocity(30);
    driveControl.moveDistance(-0.4_ft);
    driveControl.setMaxVelocity(50);
    intakeStop();
    return;
}

void oldSkills()
{
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

void absurdSkills()
{
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

void oneTower()
{
    driveControl.setMaxVelocity(90);

    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);
    // Rolls out to dispense tray
    intakeOut();
    intakeOut();
    pros::delay(400);
    intakeIn();
    pros::delay(500);

    // Drive between cubes to tower
    driveControl.moveDistance(3.5_ft);
    intakeStop();
    liftUp(false);
    driveControl.setMaxVelocity(63);
    driveControl.turnAngle(13_deg * autonColor);
    driveControl.setMaxVelocity(30);
    driveControl.moveDistance(0.3_ft);
    intakeSpit();
    driveControl.setMaxVelocity(63);
    driveControl.moveDistance(-1.1_ft);
    liftDown(true);
    driveControl.turnAngle(-165_deg * autonColor);
    intakeIn();
    driveControl.setMaxVelocity(90);
    driveControl.moveDistance(2_ft);
    intakeStop();
    driveControl.turnAngle(25_deg * autonColor);
    intakeOut();
    driveControl.moveDistance(0.17_ft);
    return;
}

void oneCube()
{
    // ONE CUBE SPIT
    
    foldOut(false);
    driveControl.setMaxVelocity(90);
    driveControl.moveDistance(-1.5_ft);
}

void yeetCube()
{
    driveControl.setMaxVelocity(90);

    trayControl.setMaxVelocity(40);
    liftControl.setMaxVelocity(200);
    // Rolls out to dispense tray
    intakeOut();
    intakeOut();
    pros::delay(400);
    intakeIn();
    pros::delay(500);
    liftUp(false);
    intakeOut();
    pros::delay(1500);
    intakeStop();
}

void turnTest()
{
    foldOut(true);
    // driveControl.setMaxVelocity(65);
    // driveControl.turnAngle(90_deg * autonColor);
}

void autonomous()
{
    autoLED = 5;

    intakeL.set_brake_mode(MOTOR_BRAKE_HOLD);
    intakeR.set_brake_mode(MOTOR_BRAKE_HOLD);
    pros::delay(30);
    if (autonType == 0)
    {
        fiveStack();
        // oneCube();
    }
    else if (autonType == 1)
    {
        sixStraight();
    }
    else if (autonType == 2)
    {
        sixProtected();
    }
    else if (autonType == 3)
    {
        eightStack();
    }
    else if (autonType == 4)
    {
        oneTower();
    }
    else if (autonType == 5)
    {
        oneCube();
    }
    else if (autonType == 6)
    {
        turnTest();
    }
    else if (autonType == 7)
    {
        absurdSkills();
    }

    return;
}