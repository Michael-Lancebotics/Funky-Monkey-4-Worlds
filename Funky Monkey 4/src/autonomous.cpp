#include "robot.hpp"

void Auton::start(){
  programmingSkills();
}

void Auton::select(){}

void Auton::programmingSkillsSetup(){
  frontFourBar.claw.open();
  transmission.fourBar.claw.close();
  transmission.setState(TransmissionState::rollers);
  frontFourBar.setState(LiftTargets::down);
  transmission.fourBar.setState(LiftTargets::down);
  fourBarTask = new pros::Task(fourBarController);
  // transmissionTask = new pros::Task(transmissionController);
  // transmissionManagerTask = new pros::Task(transmissionManager);
}

void Auton::fourBarController(){
  frontFourBar.run();
}

void Auton::transmissionController(){
  transmission.run();
}

void Auton::programmingSkills(){
  odom.reset(11.75, 115.5, 0);
  transmission.fourBar.claw.open();
  base.driveToDistance(-2.5, 0, 5, true, 60);
  transmission.fourBar.claw.close();
  base.arcToPoint(39.22, 126.4, 128, 1, false, 20, 127, true, false);
  base.driveToPoint(61, 112, 1, false, 20, 127, false, false);
  frontFourBar.claw.close();
  frontFourBar.setState(LiftTargets::score);
  // base.arcToPoint(39.22, 126.4, 128, 1, false, 20, 127, false, true);
}
