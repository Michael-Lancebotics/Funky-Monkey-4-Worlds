#include "main.h"
#include "base.hpp"

class Rollers{
private:
  Motor motor;
  int presetSpeed;

public:
  Rollers(){}
  Rollers(Motor imotor){
    motor = imotor;
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

  void setMotorPort(int iport){
    motor.setPort(iport);
  }

  void setPresetSpeed(int ispeed){
    presetSpeed = ispeed;
  }

  void move(int ispeed);
  void run();
};
