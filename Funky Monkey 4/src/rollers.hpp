#include "main.h"
#include "base.hpp"

enum class RollersState{ stop, start, reverse, smart };

class Rollers{
private:
  Motor motor;
  int presetSpeed;
  RollersState state;

public:
  Rollers(){}
  Rollers(Motor imotor){
    motor = imotor;
  }

  RollersState getState(){
    return state;
  }

  int getMotorPort(){
    return motor.getPort();
  }

  int getPresetSpeed(){
    return presetSpeed;
  }

  int getActualSpeed(){
    return motor.getActualSpeed();
  }

  void setState(RollersState istate){
    state = istate;
  }

  void setMotorPort(int iport){
    motor.setPort(iport);
  }

  void setPresetSpeed(int ispeed){
    presetSpeed = ispeed;
  }

  void move(int ispeed);
  void run();
};
