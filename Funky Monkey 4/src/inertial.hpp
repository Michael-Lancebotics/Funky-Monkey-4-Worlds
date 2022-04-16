#include "main.h"

class Inertial{
private:
  int port;
  double rotation;
  double pitch;
  double roll;

  pros::Task *filterTask = nullptr;

public:
  Inertial(){}
  Inertial(int iport){
    port = iport;
    filterTask = new pros::Task(filter);
  }

  int getPort(){
    return port;
  }

  int getRawRotation(){
    return pros::IMU(port).get_rotation();
  }

  int getRawPitch(){
    return pros::IMU(port).get_pitch();
  }

  int getRawRoll(){
    return pros::IMU(port).get_roll();
  }

  double getRotation(){
    return rotation;
  }

  double getPitch(){
    return pitch;
  }

  double getRoll(){
    return roll;
  }

  void setRotation(double irotation){
    rotation = irotation;
  }

  void setPitch(double ipitch){
    rotation = ipitch;
  }

  void setRoll(double iroll){
    rotation = iroll;
  }

  void setPort(int iport){
    port = iport;
  }

  void calibrate();
  static void filter();
};
