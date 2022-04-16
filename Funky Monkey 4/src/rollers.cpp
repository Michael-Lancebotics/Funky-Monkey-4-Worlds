#include "robot.hpp"

void Rollers::move(int pwr){
  motor.move(pwr);
}

void Rollers::smartMove(){
  long stuckTime = 0;
  while(true){
    move(127);
    if(abs(getActualSpeed()) < 10){
      stuckTime += 1;
      if(stuckTime == 10){
        move(-127);
        pros::delay(500);
      }
    }
    else{
      stuckTime = 0;
    }
    pros::delay(DELAY_TIME);
  }

 }
