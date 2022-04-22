#include "robot.hpp"

void Auton::start(){
  printConsole(auton);
  autons[auton]();
}

void Auton::select(){
  auton = 0;
  std::string print;
  while(!controller.getEnabled()){
    if(selector.getNewPress()){
      auton += 1;
      if(auton >= autons.size()){
        auton = 0;
      }
    }
    printConsole(auton);
    print = "Auton Selected: " + autonNames[auton];
    printBrainText(0, print);
    pros::delay(DELAY_TIME);
  }
}

void Auton::stop(){
  if(fourBarTask != nullptr){
    fourBarTask->remove();
    delete fourBarTask;
    fourBarTask = nullptr;
  }
  if(rollerTask != nullptr){
    rollerTask->remove();
    delete rollerTask;
    rollerTask = nullptr;
  }
}

void Auton::programmingSkillsSetup(){
  fourBar.claw.open();
  twoBar.open();
  fourBar.setState(LiftTargets::down);
  rollers.setState(RollersState::stop);
  fourBarTask = new pros::Task(fourBarController);
  rollerTask = new pros::Task(rollerController);
}

void Auton::fourBarController(){
  printConsoleText("at ur moms house 1");
  fourBar.run();
}

void Auton::rollerController(){
  printConsoleText("at ur moms house 2");
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
  printConsoleText("right side");
  odom.reset();
  base.driveToMogo(0, 40, false, 0, 50, 0.5, false, 20, 127);
  fourBar.claw.close();
  base.driveToPoint(0, 21, 3, true, 20, 127);
  fourBar.setState(LiftTargets::hover);
  pros::delay(300);
  base.turnToPoint(13.5, 17, true, 20, 80);
  fourBar.setState(LiftTargets::down);
  base.driveToMogo(13.5, 17, true, 20, 17, 2, true, 20, 50);
  fourBar.setState(LiftTargets::score);
  base.setDrive(-50, 0);
  pros::delay(300);
  twoBar.close();
  pros::delay(300);
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
  printConsoleText("left side");
  odom.reset(0, 0, 8.5);
  base.driveToMogo(sin(degToRad(8.5))*40, cos(degToRad(8.5))*40, false, sin(degToRad(8.5))*50, cos(degToRad(8.5))*50, 0.5, false, 20, 127);
  fourBar.claw.close();
  base.driveToPoint(-6, -2, 3, true, 20, 127);
  long start = pros::millis();
  while((pros::millis() - start) < 1200){
    base.setDrive(-80, 0);
    pros::delay(DELAY_TIME);
  }
  fourBar.setState(LiftTargets::hover);
  base.turnToAngle(90, true, 20, 40);
  fourBar.setState(LiftTargets::down);
  pros::delay(100);
  base.driveToMogo(4, odom.getY(), true, 15, odom.getY(), 3, true, 20, 127);
  base.setDrive(-40, 0);
  pros::delay(200);
  twoBar.close();
  pros::delay(200);
  base.setDrive(0, 0);
  fourBar.setState(LiftTargets::score);
  pros::delay(1500);
  rollers.setState(RollersState::smart);
  base.driveToPoint(-18, odom.getY(), 3, false, 20, 40);
  base.driveToPoint(-1, odom.getY(), 3, true, 20, 80);
  pros::delay(1000);
  base.driveToPoint(-18, odom.getY(), 3, false, 20, 40);
  base.driveToPoint(-1, odom.getY(), 3, true, 20, 80);
}

void Auton::winPoint(){
  printConsoleText("win point");
  odom.reset();
  base.driveToMogo(0, 40, false, 0, 50, 0.5, false, 20, 127);
  fourBar.claw.close();
  base.driveToPoint(1, 21, 1, true, 20, 127);
  fourBar.setState(LiftTargets::hover);
  pros::delay(300);
  base.turnToPoint(13.5, 17, true, 20, 80);
  fourBar.setState(LiftTargets::down);
  base.driveToMogo(13.5, 17, true, 20, 17, 2, true, 20, 50);
  fourBar.setState(LiftTargets::score);
  base.setDrive(-50, 0);
  pros::delay(400);
  twoBar.close();
  pros::delay(600);
  rollers.setState(RollersState::smart);
  base.driveToPoint(-70, 17, 0, false, 20, 127);
  twoBar.open();
  rollers.setState(RollersState::stop);
  odom.reset(odom.getY(), -odom.getX(), radToDeg(odom.getA() + (M_PI/2)));
  base.arcToPoint(-3, 90, 270, 1, false, 20, 70);
  odom.reset(-odom.getY(), odom.getX(), radToDeg(odom.getA() - (M_PI/2)));
  long start = pros::millis();
  while((pros::millis() - start) < 800){
    base.setDrive(80, 0);
    pros::delay(DELAY_TIME);
  }
  base.setDrive(0, 0);
  base.turnToAngle(90, true, 30, 60);
  // fourBar.setState(LiftTargets::hover);
  pros::delay(500);
  base.driveToMogo(-75, odom.getY(), true, -60, odom.getY(), 3, true, 20, 60);
  base.setDrive(-20, 0);
  pros::delay(200);
  twoBar.close();
  pros::delay(200);
  base.setDrive(0, 0);
  fourBar.setState(LiftTargets::score);
  pros::delay(500);
  rollers.setState(RollersState::smart);
  base.driveToPoint(-97, odom.getY(), 3, false, 20, 40);
  base.driveToPoint(-79, odom.getY(), 3, true, 20, 80);
}

void Auton::middleFromLeft(){
  printConsoleText("mid left");
}
void Auton::middleFromRight(){
  printConsoleText("mid right");
}
