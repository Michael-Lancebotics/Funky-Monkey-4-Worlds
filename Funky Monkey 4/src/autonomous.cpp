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

  #if false
  odom.reset(11.75, 115.5, 0);
  fourBar.claw.open();
  fourBar.setState(LiftTargets::hover);
  base.driveToMogo(11.75, 110.5, false, 11.75, 110.5, 0.5, true, 20, 127, true, true, false);
  twoBar.close();
  pros::delay(100);
  rollers.setState(RollersState::smart);
  base.arcToPoint(35.5, 126, 115, 1, false, 20, 127, true, true, 9);//tune basewidth if inconsistent with decel
  fourBar.setState(LiftTargets::down);
  base.driveToMogo(70.95, 110.12, true, 78.97, 106.04, 1, false, 20, 127, true, true, true);
  fourBar.claw.close();
  fourBar.setState(LiftTargets::score);
  base.arcToPoint(98.7, 89.6, 180, 1, false, 20, 127, true, true, BASE_WIDTH);
  base.driveToPoint(100, 61, 1, false, 20, 70);
  base.driveToPoint(100, 71.5, 1, true, 20, 70);
  base.turnToPoint(125, 71.5, false, 20, 80, true, true, false);
  base.driveToPoint(113, 74.5, 1);
  base.setDrive(50, 0);
  fourBar.setState(LiftTargets::platform);
  pros::delay(500);
  fourBar.claw.open();
  base.setDrive(-50, 0);
  pros::delay(500);
  fourBar.setState(LiftTargets::score);
  base.driveToPoint(105, 73.5, 1, true);
  fourBar.setState(LiftTargets::down);
  base.turnToPoint(58.24, 75.52, false, 20, 50, true, true, true);
  twoBar.open();
  pros::delay(500);
  base.driveToMogo(71.12, 75.33, true, 58.24, 75.52, 1, false, 20, 60, true, false, false);
  fourBar.claw.close();
  fourBar.setState(LiftTargets::hover);
  // pros::delay(500);
  // base.turnToPoint(125, 73.5, false, 20, 60, true, true, false);
  // base.driveToPoint(113, 76.5, 1, false, 20, 60);
  // base.setDrive(50, 0);
  // fourBar.setState(LiftTargets::platform);
  // pros::delay(500);
  // fourBar.claw.open();
  // base.setDrive(-50, 0);
  // pros::delay(500);
  // fourBar.setState(LiftTargets::score);
  // pros::delay(500);
  // base.turnToPoint(38, 73.5, false, 20, 80, true, true, false);
  base.driveToPoint(45, 37, 1, false);
  //___int_least16_t_defined
  // fourBar.move(10);
  // fourBar.setState(LiftTargets::platform);
  base.turnToAngle(180, true, 40, 127);
  base.driveToPoint(41, 45, 1, false, 20, 70);
  fourBar.setState(LiftTargets::down);
  fourBar.claw.open();
  base.driveToMogo(41, 10, true, 41, 0, 1, true, 20, 80, true, true);
  base.setDrive(-50, 0);
  pros::delay(200);
  twoBar.close();
  pros::delay(200);
  base.setDrive(0, 0);
  base.driveToPoint(41, 34, 1, false, 20, 70);

  fourBar.setState(LiftTargets::score);
  base.turnToAngle(270, false, 20, 127);
  base.driveToPoint(17, 34, 1, false, 20, 50);
  base.setDrive(127, 0);
  pros::delay(500);
  base.setDrive(0, 0);
  pros::delay(1250);

  long duration = pros::millis() - start;
  printBrain(3, duration);

  #endif

  ///////////////////////////////temp

  twoBar.close();
  fourBar.setState(LiftTargets::score);
  rollers.setState(RollersState::smart);
  odom.reset(0, 0, 270);
  pros::delay(1000);



  ///////////////////////////////
  long start = pros::millis();

  // double newX = abs(milimetersToInches(odom.left.getFilteredDistance()) - 36) > 5 ? 34 : milimetersToInches(odom.left.getFilteredDistance())
  odom.reset(17, milimetersToInches(odom.left.getFilteredDistance()) + 5.75);

  base.driveToPoint(41, 36, 1, true, 20, 127);
  fourBar.setState(LiftTargets::down);
  base.turnToPoint(80, 36, false, 20, 80);
  printBrain(0, fourBar.claw.);
  // base.driveToMogo(70, 36, true, 80, 36, 1, false, 20, 127, true, false, true);
  // fourBar.setState(LiftTargets::score);
  // base.driveToPoint(123, 57, 1, false, 20, 50);
  // fourBar.claw.open();
  // base.setDrive(-50, 0);
  // pros::delay(500);
  // base.setDrive(0, 0);






}

void Auton::rightSide(){
  printConsoleText("right side");
  odom.reset(0, 5, 0);
  base.driveToMogo(0, 40, false, 0, 50, 0.5, false, 20, 127);
  fourBar.claw.close();
  base.driveToPoint(0, 21, 1, true, 20, 127);
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
  odom.reset(5*sin(degToRad(8.5)), 5*cos(degToRad(8.5)), 8.5);
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
  odom.reset(0, 5, 0);
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
  base.driveToMogo(-75, odom.getY(), true, -60, odom.getY(), 0.5, true, 20, 60);
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

void Auton::rightMiddle(){
  long start = pros::millis();
  odom.reset(0, 5, 0);
  base.driveToMogo(0, 40, false, 0, 50, 0.5, false, 20, 127);
  fourBar.claw.close();
  base.driveToPoint(-4, 30, 1, true, 20, 127, true, false);
  fourBar.claw.open();
  double mogoX = odom.getX() + 15*sin(odom.getA());
  double mogoY = odom.getY() + 15*cos(odom.getA());
  base.driveToPoint(-4, 12, 1, true, 20, 127);
  base.turnToPoint(-19, 39);
  base.driveToMogo(-19, 39, false, -30, 44, 0.5, false, 20, 127);
  printBrain(2, odom.getX());
  printBrain(3, odom.getY());
  fourBar.claw.close();
  base.driveToPoint(1, 21, 1, true, 20, 127);
  fourBar.claw.open();
  if(radToDeg(odom.getA() - findAngle(odom.getX(), odom.getY(), 13.5, 17)) > 20){
    printBrainText(6, "turned");
    base.turnToPoint(13.5, 17, true, 20, 80);
  }
  base.driveToMogo(13.5, 17, true, 20, 17, 2, true, 20, 50);
  base.setDrive(-50, 0);
  pros::delay(300);
  twoBar.close();
  pros::delay(300);
  base.setDrive(0, 0);
  base.turnToPoint(mogoX, mogoY);
  base.driveToMogo(mogoX, mogoY, true, mogoX - 10, mogoY + 10, 0.5, false, 20, 70, true, true, false);
  fourBar.claw.close();
  fourBar.setState(LiftTargets::score);
  rollers.setState(RollersState::smart);
  pros::delay(200);
  base.turnToPoint(11, -8);
  base.driveToPoint(11, -8, 1, false, 20, 60);
  base.driveToPoint(11, 5, 1, true, 20, 60);
  base.turnToAngle(90);
  fourBar.setState(LiftTargets::down);
  long duration = pros::millis() - start;
  printBrain(2, duration);
}

void Auton::middleFromLeft(){
  printConsoleText("mid left");
}
void Auton::middleFromRight(){
  printConsoleText("mid right");
}
