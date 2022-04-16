#include "main.h"
#include "piston.hpp"
#include "distance.hpp"

class Claw{
private:
  Piston piston;
  Distance leftDistance;
  Distance rightDistance;

public:
  Claw(){}
  Claw(Piston ipiston, Distance ileftDistance, Distance irightDistance){
    piston = ipiston;
    leftDistance = ileftDistance;
    rightDistance = irightDistance;
  }

  PistonState getState(){
    return piston.getState();
  }

  bool hasMogo();
  int getAverageDistance();
  void switchState();
  void open();
  void close();
  double getExpectedDistance(double mogoX, double mogoY, bool front);
  int mogoAligned(double mogoX, double mogoY, bool front);
};
