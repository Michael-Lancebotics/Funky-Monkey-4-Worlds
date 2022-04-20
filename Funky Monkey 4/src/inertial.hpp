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

  double getRawRotation(){
    return pros::IMU(port).get_rotation();
  }

  double getRawPitch(){
    return pros::IMU(port).get_pitch();
  }

  double getRawRoll(){
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
  void start();
  void stop();
};
