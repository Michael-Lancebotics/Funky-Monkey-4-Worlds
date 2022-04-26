#include "robot.hpp"

void initialize(){
	pros::lcd::initialize();
	base.hold();
	fourBar.hold();
	twoBar.open();
	
	// fourBar.setup();
	// auton.select();
	// printConsole(controller.getCompetitionSwitch());
	odom.calibrate();
	auton.programmingSkillsSetup();
}

void disabled(){}

void competition_initialize(){}

void autonomous(){
	auton.programmingSkills();
	// auton.start();
}

void opcontrol(){
	// auton.stop();
	driveControl.start();
	// while(true){
	// 	printConsole(twoBar.hasMogo());
	// 	printConsole(twoBar.getDistance());
	// 	pros::delay(DELAY_TIME);
	// }
}
