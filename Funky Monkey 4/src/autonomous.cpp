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

void Auton::rightSide(){
  long ayo = pros::millis();
  odom.reset();
  base.driveToMogo(0, 46, false, 0, 48, 2, false, 127, 70);
  long dababy = pros::millis() - ayo;
  printBrain(6, dababy);
  base.driveToPoint(0, 21, 3, true, 127, 70);
  fourBar.setState(LiftTargets::hover);
  pros::delay(200);
  base.turnToAngle(90, true);
  // robot.motorControl.turnToPoint(13.5, 19, true, 80, 20);
  base.driveToPoint(13.5, 20.5, 2, true, 80, 20);
  fourBar.setState(LiftTargets::score);
  base.setDrive(-50, 0);
  pros::delay(600);
  twoBar.close();
  base.driveToPoint(11, 20, 0, false, 127, 20);
  base.turnToPoint(odom.getX(), 55);
  rollers.setState(RollersState::smart);
  pros::delay(200);
  base.driveToPoint(odom.getX(), 55, 1, false, 40, 20);
  base.driveToPoint(odom.getX(), 25, 5, true, 60, 20);
  base.turnToAngle(180);
  base.driveToPoint(odom.getX(), 5, 5, true, 60, 20);
  base.driveToPoint(odom.getX(), 20, 5, true, 60, 20);
  base.turnToAngle(90);
  fourBar.setState(LiftTargets::down);
}
