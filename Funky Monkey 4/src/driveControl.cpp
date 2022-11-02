#include "robot.hpp"

void DriveControl::start(){
  baseTask = new pros::Task(baseController);
  fourBarTask = new pros::Task(fourBarController);
  fourBarManagerTask = new pros::Task(fourBarManager);
  twoBarTask = new pros::Task(twoBarController);
  rollerTask = new pros::Task(rollerController);
  rollerManagerTask = new pros::Task(rollerManager);
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
  if(rollerManagerTask != nullptr){
    rollerManagerTask->remove();
    delete rollerManagerTask;
    rollerManagerTask = nullptr;
  }
}

void DriveControl::baseController(){
  base.coast();
  double pwrY = 0;
  double pwrA = 0;
  double totalPwr = 0;
  bool coast = true;
  while(true){
    if(fourBar.claw.getState() == PistonState::retract){
      printBrainText(4, "retract");
    }
    else if(fourBar.claw.getState() == PistonState::extend){
      printBrainText(4, "extend");
    }
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
      if(fourBar.claw.getState() == PistonState::extend){
        controller.rumble(".");
      }
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

void DriveControl::rollerManager(){
  bool reverse = false;
  while(true){
    if(controller.getNewPress(DOWN)){
      reverse = !reverse;
    }
    if(reverse){
      rollers.setState(RollersState::reverse);
    }
    else{
      rollers.setState(RollersState::smart);
    }
  }
}

void DriveControl::rollerController(){
  rollers.setState(RollersState::smart);
  rollers.run();
}
