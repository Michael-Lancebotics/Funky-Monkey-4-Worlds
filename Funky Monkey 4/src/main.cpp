#include "robot.hpp"

void initialize(){
	pros::lcd::initialize();
	// auton.select();
	base.hold();
	frontFourBar.hold();
	transmission.hold();
	odom.calibrate();
	// auton.programmingSkillsSetup();
}

void disabled(){}

void competition_initialize(){}

void autonomous(){
	// auton.start();
	base.driveToPoint(0, 24, 1);
	pros::delay(500);
	printConsole(odom.getX());
	printConsole(odom.getY());
	printConsole(odom.getA());
}

void opcontrol(){
	driveControl.start();
}
