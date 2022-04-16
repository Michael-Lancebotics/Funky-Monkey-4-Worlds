#include "robot.hpp"

//converts radians to degrees
double radToDeg(double rad){
  return rad / M_PI * 180.0;
}

//converts radians to degrees
double degToRad(double deg){
  return deg * M_PI / 180.0;
}

//finds the equivilant angle between 0 and 2pi radians
double angleInRange(double angle){
  while(angle >= M_PI){
    angle -= 6.28318530718;
  }
  while(angle < -M_PI){
    angle += 6.28318530718;
  }
  return angle;
}

//finds the distance between to points
double findDist(const double& x1, const double& y1, const double& x2, const double& y2){
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

//returns the angle between two points in degrees
double findAngle(const double& x1, const double& y1, const double& x2, const double& y2){
  double xDist = x2-x1;
  double yDist = y2-y1;
  return angleInRange(atan2(xDist, yDist));
}

//finds the sign of a number
int sgn(const double& num){
  if (num < 0){
    return -1;
  }
  else if (num > 0){
    return 1;
  }
  else{
    return 0;
  }
}

int boolToSgn(bool sgn){
  return sgn*2-1;
}

bool sgnToBool(int sgn){
  return (sgn+1)/2;
}

//sets a max to a value
double setMax(double num, const double& limit){
  return ((num > limit) ? limit : num);
}

//sets a min to a value
double setMin(double num, const double& limit){
  return ((num < limit) ? limit : num);
}

double inchesToMilimeters(double inches){
  return inches * 25.4;
}

double milimetersToInches(double mm){
  return mm / 25.4;
}
