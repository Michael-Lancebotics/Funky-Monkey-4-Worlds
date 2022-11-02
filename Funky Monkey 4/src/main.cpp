#include "robot.hpp"

void initialize(){
	pros::lcd::initialize();
	base.hold();
	fourBar.hold();
	twoBar.open();

	// fourBar.setup();
	// auton.select();
	// printConsole(controller.getCompetitionSwitch());
	auton.setAuton(0);
	// auton.programmingSkills();
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
