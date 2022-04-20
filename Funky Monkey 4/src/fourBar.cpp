#include "robot.hpp"

void FourBar::setup(){
  calibrateTask = new pros::Task(calibrate);
}

void FourBar::move(int pwr){
  motor.move(pwr);
}

void FourBar::run(){
  int error = static_cast<int>(target) - rotation.getValue();
  double kP = 0.03 + (sgnToBool(sgn(error)) ? (claw.hasMogo() ? 0.05 : 0) : (claw.hasMogo() ? -0.02 : -0.01));
  int pwr = 0;
  while(true){
    error = static_cast<int>(getState()) - getPosition();
    if(abs(error) > 1000){
      kP = 0.03 + (sgnToBool(sgn(error)) ? (claw.hasMogo() ? 0.05 : 0) : (claw.hasMogo() ? -0.02 : -0.01));
      pwr = error*kP;
      if(abs(pwr) < 50 && sgnToBool(sgn(error)) && claw.hasMogo()){
        pwr = 50*sgn(pwr);
      }
      else if(abs(pwr) < 20){
        pwr = 20*sgn(pwr);
      }
      move(pwr);
    }
    else{
      pwr = 0;
      move(claw.hasMogo() ? 10 : 0);
    }
    pros::delay(DELAY_TIME);
  }
}

void FourBar::calibrate(){
  fourBar.move(-90);
  pros::delay(200);
  while(abs(fourBar.motor.getActualSpeed()) > 0){
    pros::delay(DELAY_TIME);
  }
  fourBar.rotation.reset();
}

void FourBar::moveToTarget(){
  int error = static_cast<int>(target) - rotation.getValue();
  double kP = 0.3 + (sgnToBool(sgn(error)) ? (claw.hasMogo() ? 0.5 : 0) : (claw.hasMogo() ? -0.2 : -0.1));
  int pwr = 0;
  while(abs(error) > 200){
    error = static_cast<int>(target) - rotation.getValue();
    kP = 0.3 + (sgnToBool(sgn(error)) ? (claw.hasMogo() ? 0.5 : 0) : (claw.hasMogo() ? -0.2 : -0.1));
    pwr = error*kP;
    if(abs(pwr) < 50 && sgnToBool(sgn(error)) && claw.hasMogo()){
      pwr = 50;
    }
    else if(abs(pwr) < 20){
      pwr = 20;
    }
    move(pwr);
    pros::delay(DELAY_TIME);
  }
  move(claw.hasMogo() ? 10 : 0);
}

void FourBar::raiseWhenDetected(){
  while(!claw.hasMogo()){
    pros::delay(DELAY_TIME);
  }
  target = LiftTargets::score;
  moveToTarget();
}
