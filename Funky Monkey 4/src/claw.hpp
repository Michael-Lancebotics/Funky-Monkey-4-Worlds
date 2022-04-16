#include "main.h"
#include "piston.hpp"
#include "distance.hpp"

#define FRONT_CLAW_ALIGNER_LOCAL_Y_OFFSET 2
#define BACK_CLAW_ALIGNER_LOCAL_Y_OFFSET 3.5

class Claw{
private:
  double localYAlignerOffset;
  Piston piston;
  Distance distance;
  Distance leftAlignerDistance;
  Distance rightAlignerDistance;

public:
  Claw(){}
  Claw(Piston ipiston, Distance idistance, Distance ileftAlignerDistance, Distance irightAlignerDistance, double ilocalYAlignerOffset){
    piston = ipiston;
    distance = idistance;
    leftAlignerDistance = ileftAlignerDistance;
    rightAlignerDistance = irightAlignerDistance;
    localYAlignerOffset = ilocalYAlignerOffset;
  }

  PistonState getState(){
    return piston.getState();
  }

  bool hasMogo();
  int getAverageDistance();
  int getDistance();
  void switchState();
  void open();
  void close();
  double getExpectedDistance(double mogoX, double mogoY);
  int mogoAligned(double mogoX, double mogoY);
};
