#include "robot.hpp"

void DriveControl::start(){
  baseTask = new pros::Task(baseController);
  fourBarTask = new pros::Task(fourBarController);
  fourBarManagerTask = new pros::Task(fourBarManager);
  transmissionTask = new pros::Task(transmissionController);
  transmissionManagerTask = new pros::Task(transmissionManager);
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
  if(transmissionTask != nullptr){
    transmissionTask->remove();
    delete transmissionTask;
    transmissionTask = nullptr;
  }
  if(transmissionManagerTask != nullptr){
    transmissionManagerTask->remove();
    delete transmissionManagerTask;
    transmissionManagerTask = nullptr;
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
      if(frontFourBar.getState() == LiftTargets::down){
        frontFourBar.setState(LiftTargets::hover);
        printBrainText(6, "hover");
      }
      else if(frontFourBar.getState() == LiftTargets::hover || frontFourBar.getState() == LiftTargets::platform){
        frontFourBar.setState(LiftTargets::score);
        printBrainText(6, "score");
      }
    }
    else if(controller.getNewPress(R2)){
      if(frontFourBar.getState() == LiftTargets::score){
        frontFourBar.setState(LiftTargets::platform);
        printBrainText(6, "platform");
      }
      else if(frontFourBar.getState() == LiftTargets::platform || frontFourBar.getState() == LiftTargets::hover){
        frontFourBar.setState(LiftTargets::down);
        printBrainText(6, "down");
      }
    }
    if(controller.getNewPress(A)){
      frontFourBar.claw.switchState();
    }
    pros::delay(DELAY_TIME);
  }
}

void DriveControl::fourBarController(){
  frontFourBar.run();
}

void DriveControl::transmissionController(){
  transmission.run();
}

void DriveControl::transmissionManager(){
  transmission.fourBar.setPosition(0);
  while(true){
    if(transmission.ringable()){
      printBrainText(5, "rollers");
      if(transmission.getState() != TransmissionState::rollers)transmission.setState(TransmissionState::rollers);
      transmission.fourBar.setState(LiftTargets::down);
      if(controller.getNewPress(L1)){
        transmission.fourBar.setState(LiftTargets::hover);
      }
      transmission.fourBar.setPosition(0);
    }
    else{
      printBrainText(5, "fourBar");
      if(transmission.getState() != TransmissionState::fourBar)transmission.setState(TransmissionState::fourBar);
      if(controller.getNewPress(L1)){
        if(transmission.fourBar.getState() == LiftTargets::down){
          transmission.fourBar.setState(LiftTargets::hover);
        }
        else if(transmission.fourBar.getState() == LiftTargets::hover || transmission.fourBar.getState() == LiftTargets::platform){
          transmission.fourBar.setState(LiftTargets::score);
        }
      }
      else if(controller.getNewPress(L2)){
        if(transmission.fourBar.getState() == LiftTargets::score){
          transmission.fourBar.setState(LiftTargets::platform);
        }
        else if(transmission.fourBar.getState() == LiftTargets::platform || transmission.fourBar.getState() == LiftTargets::hover){
          transmission.fourBar.setState(LiftTargets::down);
        }
      }
    }
    printBrainText(4, "here");
    if(controller.getNewPress(B)){
      printBrainText(5, "here");
      transmission.fourBar.claw.switchState();
    }
    pros::delay(DELAY_TIME);
  }
}
