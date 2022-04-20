#include "robot.hpp"

void Auton::start(){
  // programmingSkills();
  rightSide();
  // leftSide();
}

void Auton::select(){}

void Auton::programmingSkillsSetup(){
  fourBar.claw.open();
  twoBar.open();
  fourBar.setState(LiftTargets::down);
  rollers.setState(RollersState::stop);
  fourBarTask = new pros::Task(fourBarController);
  rollerTask = new pros::Task(rollerController);
}

void Auton::fourBarController(){
  fourBar.run();
}

void Auton::rollerController(){
  rollers.run();
}

void Auton::programmingSkills(){
  odom.reset(11.75, 115.5, 0);
  // transmission.fourBar.claw.open();
  base.driveToDistance(-2.5, 0, 5, true, 60);
  // transmission.fourBar.claw.close();
  base.arcToPoint(39.22, 126.4, 128, 1, false, 20, 127, true, false);
  base.driveToPoint(61, 112, 1, false, 20, 127, false, false);
  fourBar.claw.close();
  fourBar.setState(LiftTargets::score);
  // base.arcToPoint(39.22, 126.4, 128, 1, false, 20, 127, false, true);
}

void Auton::rightSide(){
  odom.reset();
  base.driveToMogo(0, 40, false, 0, 50, 0.5, false, 20, 127);
  fourBar.claw.close();
  base.driveToPoint(0, 21, 3, true, 20, 127);
  fourBar.setState(LiftTargets::hover);
  pros::delay(300);
  fourBar.setState(LiftTargets::down);
  base.turnToPoint(13.5, 17, true, 20, 80);
  printBrain(3, odom.getX());
  printBrain(4, odom.getY());
  base.driveToMogo(13.5, 17, true, 20, 17, 2, true, 20, 50);
  fourBar.setState(LiftTargets::score);
  base.setDrive(-50, 0);
  pros::delay(600);
  twoBar.close();
  pros::delay(300);
  printBrain(0, odom.getX());
  base.driveToPoint(15, 20, 0, false, 20, 60);
  base.turnToAngle(0);
  rollers.setState(RollersState::smart);
  pros::delay(200);
  base.driveToPoint(11, 55, 1, false, 20, 40);
  base.driveToPoint(11, 25, 5, true, 20, 60);
  base.turnToAngle(180, false, 15, 70);
  base.driveToPoint(11, -8, 1, false, 20, 60);
  base.driveToPoint(11, 5, 1, true, 20, 60);
  base.turnToAngle(90);
  fourBar.setState(LiftTargets::down);
}

void Auton::leftSide(){
  odom.reset(0, 0, 8.5);
  // base.driveToMogo(0, 40, false, 0, 50, 0.5, false, 20, 127);
  // fourBar.claw.close();
  base.driveToMogo(sin(degToRad(8.5))*40, cos(degToRad(8.5))*40, false, sin(degToRad(8.5))*48, cos(degToRad(8.5))*48, 0.5, false, 20, 127);//move 50 at 8.5 deg
  fourBar.claw.close();
// base.driveToPoint(-6, -2, 3, true, 127, 50);
// fourBar.setState(LiftTargets::hover);
// base.turnToPoint(4, -6, true, 80, 20);
// base.driveToPoint(4, -6, 3, true, 127, 50);
// base.setDrive(-40, 0);
// pros::delay(200);
// twoBar.close();
// pros::delay(200);
// rollers.setState(RollersState::smart);
// base.driveToPoint(-16, -6, 3, false, 60, 20);
// base.driveToPoint(4, -6, 3, true, 60, 20);
// base.driveToPoint(-16, -6, 3, false, 60, 20);
// base.driveToPoint(4, -6, 3, true, 60, 20);
}
