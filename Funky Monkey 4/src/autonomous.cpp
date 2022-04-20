#include "robot.hpp"

void Auton::start(){
  // programmingSkills();
  rightSide();
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

void Auton::test(){
  pros::delay(900);
  fourBar.claw.close();
}

void Auton::rightSide(){
  odom.reset();
  // testTask = new pros::Task(test);
  base.driveToMogo(0, 40, false, 0, 50, 0.5, false, 20, 127);
  fourBar.claw.close();
  base.driveToPoint(0, 21, 3, true, 20, 127);
  fourBar.setState(LiftTargets::hover);
  pros::delay(300);
  base.turnToAngle(90, true, 20, 90);
  // robot.motorControl.turnToPoint(13.5, 19, true, 80, 20);
  base.driveToPoint(13.5, 20.5, 2, true, 20, -50);
  fourBar.setState(LiftTargets::score);
  base.setDrive(-50, 0);
  pros::delay(600);
  twoBar.close();
  base.driveToPoint(11, 20, 0, false, 20, 127);
  base.turnToAngle(0);
  rollers.setState(RollersState::smart);
  pros::delay(200);
  base.driveToPoint(11, 55, 1, false, 20, 40);
  base.driveToPoint(11, 25, 5, true, 20, 60);
  base.turnToAngle(180, false, 15, 70);
  base.driveToPoint(11, -10, 1, false, 20, 60);
  base.driveToPoint(11, 20, 1, true, 20, 60);
  // base.turnToAngle(90);
  fourBar.setState(LiftTargets::down);
}
