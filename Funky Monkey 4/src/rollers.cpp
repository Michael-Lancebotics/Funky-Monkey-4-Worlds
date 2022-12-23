#include "robot.hpp"

void Rollers::move(int pwr){
  motor.move(pwr);
}

void Rollers::run(){
  int stuckTime = 0;
  bool wasRunning = false;
  while(true){
    switch(state){
      case RollersState::stop:
      rollers.move(0);
      stuckTime = 0;
      wasRunning = false;
      break;

      case RollersState::start:
      rollers.move(127);
      stuckTime = 0;
      wasRunning = false;
      break;

      case RollersState::reverse:
      rollers.move(-127);
      stuckTime = 0;
      wasRunning = false;
      break;

      case RollersState::smart:
      if(twoBar.getState() == PistonState::extend && twoBar.hasMogo() && fourBar.getState() != LiftTargets::down){//FourBarRotation.get_position() > -20400 &&
        if(!wasRunning){
          wasRunning = true;
        }
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
        wasRunning = false;
      }
      break;
    }
    pros::delay(DELAY_TIME);
  }
}
