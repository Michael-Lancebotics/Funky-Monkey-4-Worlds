#include "robot.hpp"

void initialize(){
	pros::lcd::initialize();
	// auton.select();
	// base.hold();
	// fourBar.hold();
	// fourBar.setup();
	odom.calibrate();
	auton.programmingSkillsSetup();
}

void disabled(){}

void competition_initialize(){}

void autonomous(){
	auton.start();
}

void opcontrol(){
	driveControl.start();
}
