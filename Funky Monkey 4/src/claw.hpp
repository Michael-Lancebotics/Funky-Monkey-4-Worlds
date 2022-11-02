#include "main.h"
#include "piston.hpp"
#include "distance.hpp"

#define FRONT_CLAW_ALIGNER_LOCAL_Y_OFFSET 2
#define BACK_CLAW_ALIGNER_LOCAL_Y_OFFSET 3.5

#define FRONT_CLAW_MAX_GRAB_DISTANCE 25
#define FRONT_CLAW_MIN_GRAB_DISTANCE 0
#define BACK_CLAW_MAX_GRAB_DISTANCE 38
#define BACK_CLAW_MIN_GRAB_DISTANCE 10

class Claw{
private:
  double localYAlignerOffset;
  Piston piston;
  Distance distance;
  int maxGrabDistance;
  int minGrabDistance;

public:
  Distance leftAlignerDistance;
  Distance rightAlignerDistance;
  Claw(){}
  Claw(Piston ipiston, Distance idistance, Distance ileftAlignerDistance, Distance irightAlignerDistance, double ilocalYAlignerOffset, int iminGrabDistance, int imaxGrabDistance){
    piston = ipiston;
    distance = idistance;
    leftAlignerDistance = ileftAlignerDistance;
    rightAlignerDistance = irightAlignerDistance;
    localYAlignerOffset = ilocalYAlignerOffset;
    minGrabDistance = iminGrabDistance;
    maxGrabDistance = imaxGrabDistance;
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
