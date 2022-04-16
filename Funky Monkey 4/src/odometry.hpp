#include "main.h"
#include "inertial.hpp"
#include "trackingWheel.hpp"
#include "transmission.hpp"

class Odometry{
private:
  double x;
  double y;
  double a;
  double velX;
  double velY;
  double velA;

  TrackingWheel xWheel;
  TrackingWheel yWheel;

  Distance left;
  Distance right;
  Distance frontLeft;
  Distance frontRight;

  pros::Task *updateTask = nullptr;
  pros::Task *distanceFilterTask = nullptr;

public:
  Inertial inertial;

  Odometry(TrackingWheel ixWheel, TrackingWheel iyWheel, Inertial iinertial, Distance ileft, Distance iright, Distance ifrontLeft, Distance ifrontRight){
    xWheel = ixWheel;
    yWheel = iyWheel;
    inertial = iinertial;
    left = ileft;
    right = iright;
    frontLeft = ifrontLeft;
    frontRight = ifrontRight;
  }

  double getX(){
    return x;
  }

  double getY(){
    return y;
  }

  double getA(){
    return a;
  }

  double getXVel(){
    return velX;
  }

  double getYVel(){
    return velY;
  }

  double getAVel(){
    return velA;
  }

  void setX(double ix){
    x = ix;
  }

  void setY(double iy){
    y = iy;
  }

  void setAngle(double ia){
    a = ia;
  }

  void setXVelocity(double ivelX){
    velX = ivelX;
  }

  void setYVelocity(double ivelY){
    velY = ivelY;
  }

  void setAngleVelocity(double ivelA){
    velA = ivelA;
  }

  void reset(double ix, double iy, double ia);
  void reset(double ix, double iy);
  void reset();
  void wallReset(bool front);
  void fullReset();
  double filter(const double& currentVal, const double& lastVal);
  void calibrate();
  void start();
  void stop();
  void startDistanceFilter();
  void stopDistanceFilter();
  static void distanceFilter();
  static void update();
};
