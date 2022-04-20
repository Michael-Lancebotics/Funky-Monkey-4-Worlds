#include "robot.hpp"

void Inertial::filter(){
  double currentRotation = odom.inertial.getRawRotation();
  double lastRotation = odom.inertial.getRawRotation();
  double filteredRotation = 0;
  double currentPitch = odom.inertial.getRawPitch();
  double lastPitch = odom.inertial.getRawPitch();
  double filteredPitch = 0;
  double currentRoll = odom.inertial.getRawRoll();
  double lastRoll = odom.inertial.getRawRoll();
  double filteredRoll = 0;
  while(true){
    currentRotation = odom.inertial.getRawRotation();
    filteredRotation = currentRotation - lastRotation;
    if(fabs(filteredRotation) < 0.001) filteredRotation = 0;
    odom.inertial.rotation += filteredRotation;
    lastRotation = currentRotation;

    currentPitch = odom.inertial.getRawPitch();
    filteredPitch = currentPitch - lastPitch;
    if(fabs(filteredPitch) < 0.001) filteredPitch = 0;
    odom.inertial.pitch += filteredPitch;
    lastPitch = currentPitch;

    currentRoll = odom.inertial.getRawRoll();
    filteredRoll = currentRoll - lastRoll;
    if(fabs(filteredRoll) < 0.001) filteredRoll = 0;
    odom.inertial.roll += filteredRoll;
    lastRoll = currentRoll;

    pros::delay(DELAY_TIME);
  }
}

void Inertial::calibrate(){
  // printConsoleText("started");

  if(filterTask != nullptr){
    filterTask->remove();
    delete filterTask;
    filterTask = nullptr;
  }

  pros::IMU(port).reset();
  while(pros::IMU(port).is_calibrating()){
    pros::delay(DELAY_TIME);
  }

  filterTask = new pros::Task(filter);
}

void Inertial::start(){
  filterTask = new pros::Task(filter);
}

void Inertial::stop(){
  if(filterTask != nullptr){
    filterTask->remove();
    delete filterTask;
    filterTask = nullptr;
  }
}
