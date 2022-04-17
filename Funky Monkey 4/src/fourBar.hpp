#include "main.h"
#include "motor.hpp"
#include "rotation.hpp"
#include "claw.hpp"

#define FRONT_FOUR_BAR_DOWN -35859
#define FRONT_FOUR_BAR_HOVER -24407
#define FRONT_FOUR_BAR_PLATFORM -5633
#define FRONT_FOUR_BAR_SCORE 20171

#define BACK_FOUR_BAR_DOWN 0
#define BACK_FOUR_BAR_HOVER 10000
#define BACK_FOUR_BAR_PLATFORM 27000
#define BACK_FOUR_BAR_SCORE 47000

enum class LiftTargets{ down, hover, platform, score };

class FourBar{
private:
  int position;
  Motor motor;
  Rotation rotation;
  LiftTargets target;
  int targets[4];

public:
  FourBar(){}
  FourBar(Motor imotor, Claw iclaw, Rotation irotation, int idown, int ihover, int iplatform, int iscore){
    motor = imotor;
    rotation = irotation;
    claw = iclaw;
    targets[0] = idown;
    targets[1] = ihover;
    targets[2] = iplatform;
    targets[3] = iscore;
  }

  void equals(const FourBar &other){
    motor = other.motor;
    rotation = other.rotation;
    claw = other.claw;
    targets[0] = other.targets[0];
    targets[1] = other.targets[1];
    targets[2] = other.targets[2];
    targets[3] = other.targets[3];
  }

  Claw claw;

  int getTarget(){
    return targets[static_cast<int>(target)];
  }

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

  void move(int pwr);
  void calibrate();
  void moveToTarget();
  void raiseWhenDetected();
  void run();

};
