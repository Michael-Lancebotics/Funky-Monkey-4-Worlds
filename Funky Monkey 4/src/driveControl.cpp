#include "robot.hpp"

void DriveControl::start(){
  baseTask = new pros::Task(baseController);
  fourBarTask = new pros::Task(fourBarController);
  fourBarManagerTask = new pros::Task(fourBarManager);
  twoBarTask = new pros::Task(twoBarController);
  rollerTask = new pros::Task(rollerController);
}

void DriveControl::stop(){
  if(baseTask != nullptr){
    baseTask->remove();
    delete baseTask;
    baseTask = nullptr;
  }
  if(fourBarTask != nullptr){
    fourBarTask->remove();
    delete fourBarTask;
    fourBarTask = nullptr;
  }
  if(fourBarManagerTask != nullptr){
    fourBarManagerTask->remove();
    delete fourBarManagerTask;
    fourBarManagerTask = nullptr;
  }
  if(twoBarTask != nullptr){
    twoBarTask->remove();
    delete twoBarTask;
    twoBarTask = nullptr;
  }
  if(rollerTask != nullptr){
    rollerTask->remove();
    delete rollerTask;
    rollerTask = nullptr;
  }
}

void DriveControl::baseController(){
  base.coast();
  double pwrY = 0;
  double pwrA = 0;
  double totalPwr = 0;
  bool coast = true;
  while(true){
    pwrY = controller.getJoystick(LY);
    pwrA = controller.getJoystick(RX);

    totalPwr = fabs(pwrY) + fabs(pwrA);

    if(totalPwr > 127){
      pwrY = (pwrY / totalPwr) * 127;
      pwrA = (pwrA / totalPwr) * 127;
    }

    base.setDrive(pwrY, pwrA);

    if(controller.getNewPress(X)){
      coast = !coast;
      if(coast){
        base.coast();
      }
      else{
        base.hold();
      }
    }

    pros::delay(DELAY_TIME);
  }
}
void DriveControl::fourBarManager(){
  while(true){
    if(controller.getNewPress(R1)){
      if(fourBar.getState() == LiftTargets::down){
        fourBar.setState(LiftTargets::hover);
        printBrainText(6, "hover");
      }
      else if(fourBar.getState() == LiftTargets::hover || fourBar.getState() == LiftTargets::platform){
        fourBar.setState(LiftTargets::score);
        printBrainText(6, "score");
      }
    }
    else if(controller.getNewPress(R2)){
      if(fourBar.getState() == LiftTargets::score){
        fourBar.setState(LiftTargets::platform);
        printBrainText(6, "platform");
      }
      else if(fourBar.getState() == LiftTargets::platform || fourBar.getState() == LiftTargets::hover){
        fourBar.setState(LiftTargets::down);
        printBrainText(6, "down");
      }
    }
    if(controller.getNewPress(A)){
      fourBar.claw.switchState();
    }
    pros::delay(DELAY_TIME);
  }
}

void DriveControl::fourBarController(){
  fourBar.run();
}

void DriveControl::twoBarController(){
  while(true){
    if(controller.getPress(L1)){
      printBrainText(0, "close");
      twoBar.close();
    }
    else if(controller.getPress(L2)){
      printBrainText(0, "open");
      twoBar.open();
    }
    pros::delay(DELAY_TIME);
  }
}

void DriveControl::rollerController(){
int stuckTime = 0;
while(true){
    if(twoBar.getState() == PistonState::extend && twoBar.hasMogo()){//FourBarRotation.get_position() > -20400 &&
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
    pros::delay(DELAY_TIME);
  }
}
