#include "robot.hpp"

void initialize(){
	pros::lcd::initialize();
	base.hold();
	fourBar.hold();
	// fourBar.setup();
	controller.getCompetitionSwitch() ? auton.select() : auton.setAuton(4);
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
	Button a(8);
	while(true){
		bool b = a.getNewPress();
		printConsole(b);
		pros::delay(DELAY_TIME);
	}
	// driveControl.start();
}
