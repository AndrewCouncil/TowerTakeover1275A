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
  {(int8_t) driveFL_port, (int8_t) driveBL_port}, {(int8_t) (-driveFR_port), (int8_t) (-driveBR_port)},
  AbstractMotor::gearset::green,
  {4_in, 10.9_in}
);

auto trayControl = AsyncControllerFactory::posIntegrated((int8_t)tray_port);
auto liftControl = AsyncControllerFactory::posIntegrated((int8_t)lift_port);

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
}

void intakeIn(){
    intakeL = 127;
    intakeR = 127;
    return;
}

void intakeOut(){
    intakeL = -127;
    intakeR = -127;
    return;
}

void intakeStop(){
    intakeL = 0;
    intakeR = 0;
    return;
}

// Spits out cube from intake
void intakeSpit(){
    intakeL = -88;
    intakeR = -88;
    pros::delay(700);
    intakeL = 0;
    intakeR = 0;
}

void dropStack(){
    intakeL = -30;
    intakeR = -30;
    trayControl.setTarget(340);
    while (!trayControl.isSettled()) {pros::delay(20);}
    trayControl.setTarget(0);
    intakeL = 0;
    intakeR = 0;
    return;
}

void autonomous() {
    
    
    driveControl.setMaxVelocity(100);
    if(autonType == 0){
        liftControl.setTarget(200);
        while (!liftControl.isSettled()) {pros::delay(20);}
        liftControl.setTarget(0);
        while (!liftControl.isSettled()) {pros::delay(20);}
        trayControl.setTarget(0);
        intakeIn();
        driveControl.moveDistance(2.5_ft);
        // pros::delay(200);
        intakeStop();
        driveControl.moveDistance(-2_ft);
        driveControl.turnAngle(autonColor * -110_deg);
        driveControl.moveDistance(2.5_ft);
        intakeOut();
        pros::delay(1500);
        driveControl.moveDistance(-2_ft);
        intakeStop();
    }
    else if (autonType == 3){
        // SKILLS AUTON

        trayControl.setMaxVelocity(75);
        liftControl.setMaxVelocity(200);
        // Rolls out to dispense tray
        intakeOut();
        pros::delay(500);
        // Pick up 4 cubes
        intakeIn();
        driveControl.moveDistance(4.1_ft);
        intakeStop();
        // Back up and turn toward corner
        driveControl.moveDistance(-2.5_ft);
        driveControl.turnAngle(135_deg);
        // Drive to goal and place
        driveControl.moveDistance(2_ft);
        dropStack();
        // Back up and turn with back towards the big goal
        driveControl.moveDistance(-1_ft);
        driveControl.turnAngle(-45_deg);
        // Back up and turn towards cube row
        driveControl.moveDistance(-2_ft);
        driveControl.turnAngle(-90_deg);
        // Pick up the cubes
        intakeIn();
        driveControl.moveDistance(1.5_ft);
        intakeStop();
        // Back up into wall
        driveControl.moveDistance(-4.1_ft);
        // Move out of wall and turn towards big goal
        driveControl.moveDistance(0.2_ft);
        driveControl.turnAngle(-90_deg);
        // Drive to big goal and drop stack
        driveControl.moveDistance(6.3_ft);
        dropStack();
        // Back up, turn to face orange cube
        driveControl.moveDistance(-2.3_ft);
        driveControl.turnAngle(90_deg);
        // Back up into wall and pick up 
        driveControl.moveDistance(-0.2_ft);
        intakeIn();
        driveControl.moveDistance(0.5_ft);
        intakeStop();
        driveControl.moveDistance(0.5_ft);
        // Turn towards tower and drive to it
        driveControl.turnAngle(90_deg);
        driveControl.moveDistance(2.2_ft);
        // Move lift to position and move up to tower
        liftControl.setTarget(600);
        while (!liftControl.isSettled()) {pros::delay(20);}
        driveControl.moveDistance(0.3_ft);
        // Put cube in tower, move back, lower lift
        intakeSpit();
        driveControl.moveDistance(-0.3_ft);
        liftControl.setTarget(0);
        // Pick up purple cube at tower base
        intakeIn();
        driveControl.moveDistance(0.3_ft);
        intakeStop();
        // Line up with red tower
        driveControl.moveDistance(-2_ft);
        driveControl.turnAngle(-90_deg);
        driveControl.moveDistance(1_ft);
        driveControl.turnAngle(-90_deg);
        // Drive up to red tower
        driveControl.moveDistance(6.7_ft);
        // Move lift to position and move up to tower
        liftControl.setTarget(600);
        while (!liftControl.isSettled()) {pros::delay(20);}
        driveControl.moveDistance(0.3_ft);
        // Put cube in tower, move back, lower lift
        intakeSpit();
        driveControl.moveDistance(-0.3_ft);
        liftControl.setTarget(0);
    }
    else if (autonType == 1){
        driveControl.turnAngle(90_deg);
    }
    
    return;
}
