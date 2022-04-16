#include "robot.hpp"

bool Transmission::ringable(){
  return fourBar.getState() == LiftTargets::down && fourBar.claw.hasMogo() && fourBar.claw.getState() == PistonState::extend && frontFourBar.getState() != LiftTargets::down;
}

void Transmission::run(){
  int error = static_cast<int>(fourBar.getTarget()) - fourBar.getPosition();
  double kP = 0.03 + (sgnToBool(sgn(error)) ? (fourBar.claw.hasMogo() ? 0.05 : 0) : (fourBar.claw.hasMogo() ? -0.02 : -0.01));
  int pwr = 0;
  long stuckTime = 0;
  while(true){
    switch(state){
      case TransmissionState::rollers:
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

        break;
      case TransmissionState::fourBar:
        error = static_cast<int>(fourBar.getTarget()) - fourBar.getPosition();
        if(abs(error) > 1000){
          kP = 0.03 + (sgnToBool(sgn(error)) ? (fourBar.claw.hasMogo() ? 0.05 : 0) : (fourBar.claw.hasMogo() ? -0.02 : -0.01));
          pwr = error*kP;
          if(abs(pwr) < 50 && sgnToBool(sgn(error)) && fourBar.claw.hasMogo()){
            pwr = 50*sgn(pwr);
          }
          else if(abs(pwr) < 20){
            pwr = 20*sgn(pwr);
          }
          // printBrain(4, pwr);
          fourBar.move(pwr);
        }
        else{
          pwr = 0;
          // printBrain(4, pwr);
          fourBar.move(pwr);//claw.hasMogo() ? 10 :
        }
        printConsole(error);
        printConsole(kP);
        printConsole(fourBar.getPosition());
        printConsole(pwr);
        break;
    }
    pros::delay(DELAY_TIME);
  }
}

void Transmission::calibrate(){
  long start = pros::millis();
  while(!bottom.getPressed() && (pros::millis() - start < 1000 || abs(fourBar.getActualSpeed()) > 0)){ //
    fourBar.move(-127);
    pros::delay(DELAY_TIME);
  }
  fourBar.move(0);

}
