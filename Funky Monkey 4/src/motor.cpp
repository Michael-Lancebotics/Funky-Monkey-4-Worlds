#include "robot.hpp"

#define HOLD MOTOR_BRAKE_HOLD
#define COAST MOTOR_BRAKE_COAST
#define BRAKE MOTOR_BRAKE_BRAKE

void Motor::move(int pwr){
  targetSpeed = pwr;
  pros::Motor(port, reverse).move(pwr);
}

void Motor::moveVelocity(int pwr){
  targetSpeed = pwr/200*127;
  pros::Motor(port, reverse).move_velocity(pwr);
}

void Motor::moveVoltage(int pwr){
  targetSpeed = pwr/100;
  pros::Motor(port, reverse).move_voltage(pwr);
}

int Motor::getActualSpeed(){
  return pros::Motor(port, reverse).get_actual_velocity()/200*127;
}

int Motor::getTargetSpeed(){
  return targetSpeed;
}

void Motor::hold(){
  pros::Motor(port, reverse).set_brake_mode(HOLD);
}

void Motor::coast(){
  pros::Motor(port, reverse).set_brake_mode(COAST);
}

void Motor::brake(){
  pros::Motor(port, reverse).set_brake_mode(BRAKE);
}
