#include "robot.hpp"

void initialize(){
	pros::lcd::initialize();
	// auton.select();
	base.hold();
	fourBar.hold();
	odom.calibrate();
	// auton.programmingSkillsSetup();
}

void disabled(){}

void competition_initialize(){}

void autonomous(){
	// auton.start();
	// odom.reset(0, 0, 180);
	base.driveToPoint(0, -10, 0.5, true);
	pros::delay(500);
	printConsole(odom.getX());
	printConsole(odom.getY());
	printConsole(odom.getA());
}

void opcontrol(){
	driveControl.start();
}
