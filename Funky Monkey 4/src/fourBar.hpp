#include "main.h"
#include "motor.hpp"
#include "rotation.hpp"
#include "claw.hpp"

enum class LiftTargets{ down = -36859, hover = -22407, platform = -5633, score = 20171};

class FourBar{
private:
  int position;
  Motor motor;
  Rotation rotation;
  LiftTargets target;
  pros::Task *calibrateTask = nullptr;

public:
  FourBar(){}
  FourBar(Motor imotor, Claw iclaw, Rotation irotation){
    motor = imotor;
    rotation = irotation;
    claw = iclaw;
  }

  void equals(const FourBar &other){
    motor = other.motor;
    rotation = other.rotation;
    claw = other.claw;
  }

  Claw claw;

  LiftTargets getState(){
    return target;
  }

  int getMotorPort(){
    return motor.getPort();
  }

  int getRotationPort(){
    return rotation.getPort();
  }

  int getPosition(){
    return rotation.getValue();
  }

  bool atPosition(LiftTargets position){
    return target == LiftTargets::down && rotation.getPort() == static_cast<int>(LiftTargets::down);
  }

  void setState(LiftTargets itarget){
    target = itarget;
  }

  void setMotorPort(int iport){
    motor.setPort(iport);
  }

  void setRotationPort(int iport){
    rotation.setPort(iport);
  }

  void setPosition(int iposition){
    rotation.setPosition(iposition);
  }

  int getActualSpeed(){
    return motor.getActualSpeed();
  }

  void hold(){
    motor.hold();
  }

  static void calibrate();
  void setup();
  void move(int pwr);
  void moveToTarget();
  void raiseWhenDetected();
  void run();
};
