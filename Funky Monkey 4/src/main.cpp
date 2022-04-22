#include "robot.hpp"

void initialize(){
	pros::lcd::initialize();
	base.hold();
	fourBar.hold();
	// fourBar.setup();
	// auton.select();
	auton.setAuton(4);
	printConsole(controller.getCompetitionSwitch());
	odom.calibrate();
	auton.programmingSkillsSetup();
}

void disabled(){}

void competition_initialize(){}

void autonomous(){
	auton.start();
}

void opcontrol(){
	// auton.stop();
	driveControl.start();
}
