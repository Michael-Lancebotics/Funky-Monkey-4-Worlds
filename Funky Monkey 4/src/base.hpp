#include "main.h"
#include "odometry.hpp"

#define BASE_WIDTH 11.75

class Base{
 private:
  Motor leftMotors[3];
  Motor rightMotors[3];

  double targetX;
  double targetY;
  double targetA;

 public:
  Base(){}
  Base(Motor ileftMotors[3], Motor irightMotors[3]){
    leftMotors[0] = ileftMotors[0];
    leftMotors[1] = ileftMotors[1];
    leftMotors[2] = ileftMotors[2];
    rightMotors[0] = irightMotors[0];
    rightMotors[1] = irightMotors[1];
    rightMotors[2] = irightMotors[2];
  }

  void setMotors(Motor ileftMotors[3], Motor irightMotors[3]){
    leftMotors[0] = ileftMotors[0];
    leftMotors[1] = ileftMotors[1];
    leftMotors[2] = ileftMotors[2];
    rightMotors[0] = irightMotors[0];
    rightMotors[1] = irightMotors[1];
    rightMotors[2] = irightMotors[2];
  }

  double getTargetX(){
    return targetX;
  }

  double getTargetY(){
    return targetY;
  }

  double getTargetA(){
    return targetA;
  }

  double findPwrY(double error, double initialError, int maxSpeed, int minSpeed, bool accel, bool decel);
  double findPwrA(double errorAngle, int maxSpeed, int minSpeed, double initError);
  double findCorrection(double error, int maxSpeed, int minSpeed);
  double getVelocity();
  void setDrive(int pwrY, int pwrA);
  void driveToPoint(double itargetX, double itargetY, double maxErrorX = 1, bool reverse = false, int minSpeed = 20, int maxSpeed = 127, bool accelerate = true, bool decelerate = true);
  void driveToDistance(double targetDistance, double itargetA, double maxErrorX = 1, bool reverse = false, int minSpeed = 20, int maxSpeed = 127, bool accelerate = true, bool decelerate = true);
  void arcToPoint(double itargetX, double itargetY, double itargetA, double maxErrorRadius = 1, bool reverse = false, int iminSpeed = 20, int imaxSpeed = 127, bool accelerate = true, bool decelerate = true);
  void turnToPoint(double itargetX, double itargetY, bool reverse = false, int minSpeed = 20, int maxSpeed = 127, bool accelerate = true, bool decelerate = true);
  void turnToAngle(double targetA, bool reverse = false, int minSpeed = 20, int maxSpeed = 127, bool accelerate = true, bool decelerate = true);
  void driveToMogo(double mogoX, double mogoY, bool correct, double itargetX, double itargetY, double maxErrorX = 1, bool reverse = false, int minSpeed = 20, int maxSpeed = 127, bool accelerate = true, bool decelerate = true);
  void hold();
  void coast();
  void brake();
  double getActualPwrY();
  double getActualPwrA();
};
