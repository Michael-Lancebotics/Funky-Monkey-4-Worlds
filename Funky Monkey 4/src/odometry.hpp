#include "main.h"
#include "inertial.hpp"
#include "trackingWheel.hpp"
#include "fourBar.hpp"

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
  Distance backLeft;
  Distance backRight;

  pros::Task *updateTask = nullptr;
  pros::Task *distanceFilterTask = nullptr;

public:
  Inertial inertial;

  Odometry(TrackingWheel ixWheel, TrackingWheel iyWheel, Inertial iinertial, Distance ileft, Distance iright, Distance ifrontLeft, Distance ifrontRight, Distance ibackLeft, Distance ibackRight){
    xWheel = ixWheel;
    yWheel = iyWheel;
    inertial = iinertial;
    left = ileft;
    right = iright;
    frontLeft = ifrontLeft;
    frontRight = ifrontRight;
    backLeft = ibackLeft;
    backRight = ibackRight;
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
