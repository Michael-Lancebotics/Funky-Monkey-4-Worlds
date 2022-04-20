#include "main.h"

#define Y_TRACKING_WHEEL_DISTANCE -0.75
#define X_TRACKING_WHEEL_DISTANCE 0.5

class TrackingWheel{
private:
  int port;
  double distance;
  double diameter;
  bool reverse;
  pros::ADIEncoder *encoder;

public:
  TrackingWheel(){}
  TrackingWheel(int iport, bool reverse, double idistance, double idiameter = 2.75){
    port = iport;
    distance = idistance;
    diameter = idiameter;
    encoder = new pros::ADIEncoder(port, port+1, reverse);
  }

  int getPort(){
    return port;
  }

  bool getReverse(){
    return reverse;
  }

  double getDistance(){
    return distance;
  }

  int getPosition(){
    return encoder->get_value();
  }

  double getDiameter(){
    return diameter;
  }

  void setPort(int iport){
    port = iport;
    encoder = new pros::ADIEncoder(port, port+1, reverse);
  }

  void setReverse(bool ireverse){
    reverse = ireverse;
    encoder = new pros::ADIEncoder(port, port+1, reverse);
  }

  void setDistance(double idistance){
    distance = idistance;
  }

  void setDiameter(double idiameter){
    diameter = idiameter;
  }

  void reset();
};
