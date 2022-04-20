#include "robot.hpp"

void initialize(){
	pros::lcd::initialize();
	// auton.select();
	base.hold();
	fourBar.hold();
	// fourBar.setup();
	odom.calibrate();
	auton.programmingSkillsSetup();
}

void disabled(){}

void competition_initialize(){}

void autonomous(){
	auton.start();
	// auton.start();
	// odom.reset(0, 0, 180);
	// base.arcToPoint(30, 30, 90, 1);
	// pros::delay(500);
	// printConsole(odom.getX());
	// printConsole(odom.getY());
	// printConsole(odom.getA());
}

void opcontrol(){
	base.turnToMogo(-25, 0);
	// while(true){
	// 	printConsole(twoBar.getDistance());
	// }
	// base.turnToAngle(40);
	// base.driveToMogo(0, -45, 0, -50, true);
	// fourBar.claw.mogoAligned(0, 24);
	driveControl.start();
}
