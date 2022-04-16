#include "robot.hpp"

int Rotation::getValue(){
  return pros::Rotation(port).get_position() * -boolToSgn(reverse);
}

void Rotation::reset(){
  pros::Rotation(port).reset();
}

void Rotation::setPosition(int pos){
  pros::Rotation(port).set_position(pos);
}
