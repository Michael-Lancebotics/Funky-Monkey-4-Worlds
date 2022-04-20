#include "robot.hpp"

void Rollers::move(int pwr){
  motor.move(pwr);
}

void Rollers::run(){
  int stuckTime = 0;
  while(true){
    switch(state){
      case RollersState::stop:
      rollers.move(0);
      stuckTime = 0;
      break;

      case RollersState::start:
      rollers.move(127);
      stuckTime = 0;
      break;

      case RollersState::reverse:
      rollers.move(-127);
      stuckTime = 0;
      break;

      case RollersState::smart:
      if(twoBar.getState() == PistonState::extend && twoBar.hasMogo() && fourBar.getState() != LiftTargets::down){//FourBarRotation.get_position() > -20400 &&
        rollers.move(127);
        if(abs(rollers.getActualSpeed()) < 10){
          stuckTime += 1;
          if(stuckTime == 10){
            rollers.move(-127);
            pros::delay(500);
          }
        }
        else{
          stuckTime = 0;
        }
      }
      else{
        rollers.move(0);
      }
      break;
    }
    pros::delay(DELAY_TIME);
  }
}
