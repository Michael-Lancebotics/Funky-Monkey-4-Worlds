#include "robot.hpp"

void initialize(){
	pros::lcd::initialize();
	base.hold();
	fourBar.hold();
	twoBar.open();

	auton.setAuton(0);
	odom.calibrate();
	auton.programmingSkillsSetup();
	twoBar.open();
}

void disabled(){}

void competition_initialize(){}

void autonomous(){
	fourBar.claw.open();
	twoBar.open();
	auton.programmingSkills();
}

void opcontrol(){
	driveControl.start();
}
