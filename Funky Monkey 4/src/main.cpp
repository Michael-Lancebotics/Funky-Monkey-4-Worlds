#include "robot.hpp"

void initialize(){
	pros::lcd::initialize();
	base.hold();
	fourBar.hold();
	// fourBar.setup();
	// auton.select();
	auton.setAuton(1);
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
	while(true){
		printConsole(twoBar.hasMogo());
		printConsole(twoBar.getDistance());
		pros::delay(DELAY_TIME);
	}
}
