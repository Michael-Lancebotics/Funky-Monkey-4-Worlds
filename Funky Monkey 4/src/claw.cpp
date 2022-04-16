#include "robot.hpp"

bool Claw::hasMogo(){
  return (leftDistance.getDistance() < 40 && leftDistance.getDistance() != 0) && (rightDistance.getDistance() < 40 && leftDistance.getDistance() != 0);
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

int Claw::getAverageDistance(){
  return (leftDistance.getDistance() + rightDistance.getDistance()) / 2;
}

double Claw::getExpectedDistance(double mogoX, double mogoY, bool front){
  double localYOffset = front ? 1.5 : 1.5;
  return findDist(odom.getX() + (localYOffset * sin(odom.getA())), odom.getY() + (localYOffset * cos(odom.getA())), mogoX, mogoY);
}

int Claw::mogoAligned(double mogoX, double mogoY, bool front){
  double expected = getExpectedDistance(mogoX, mogoY, front);
  bool left = fabs(milimetersToInches(leftDistance.getDistance()) - expected) < 5;
  bool right = fabs(milimetersToInches(rightDistance.getDistance()) - expected) < 5;
  double leftTemp = milimetersToInches(leftDistance.getDistance());
  double rightTemp = milimetersToInches(rightDistance.getDistance());
  printConsole(expected);
  printConsole(left);
  printConsole(right);
  printConsole(leftTemp);
  printConsole(rightTemp);
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
