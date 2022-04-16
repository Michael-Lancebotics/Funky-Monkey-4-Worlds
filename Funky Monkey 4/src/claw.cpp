#include "robot.hpp"

bool Claw::hasMogo(){
  return (distance.getDistance() < 40) && (distance.getDistance() != 0);
}

void Claw::open(){
  piston.extend();
}

void Claw::close(){
  piston.extend();
}

void Claw::switchState(){
  piston.switchState();
}

int Claw::getDistance(){
  return distance.getDistance();
}

int Claw::getAverageDistance(){
  return (leftAlignerDistance.getDistance() + rightAlignerDistance.getDistance()) / 2;
}

double Claw::getExpectedDistance(double mogoX, double mogoY){
  return findDist(odom.getX() + (localYAlignerOffset * sin(odom.getA())), odom.getY() + (localYAlignerOffset * cos(odom.getA())), mogoX, mogoY);
}

int Claw::mogoAligned(double mogoX, double mogoY){
  double expected = getExpectedDistance(mogoX, mogoY);
  bool left = fabs(milimetersToInches(leftAlignerDistance.getDistance()) - expected) < 5;
  bool right = fabs(milimetersToInches(rightAlignerDistance.getDistance()) - expected) < 5;
  if(left && right){
    return 2;
  }
  else if(right){
    return 1;
  }
  else if(left){
    return -1;
  }
  else{
    return 0;
  }
}
