#include "robot.hpp"

bool Button::getPress(){
  prev = pros::ADIDigitalIn(port).get_value();
  return prev;
}

bool Button::getNewPress(){
  if(!prev){
    prev = pros::ADIDigitalIn(port).get_value();
    return prev;
  }
  if(!pros::ADIDigitalIn(port).get_value()){
    prev = false;
  }
  return false;
}
