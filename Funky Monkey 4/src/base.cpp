#include "robot.hpp"

void Base::setDrive(int pwrY, int pwrA){
  for(int i = 0; i < 3; i++){
    leftMotors[i].move(pwrY + pwrA);
    rightMotors[i].move(pwrY - pwrA);
  }
}

void Base::hold(){
  for(int i = 0; i < 3; i++){
    leftMotors[i].hold();
    rightMotors[i].hold();
  }
}

void Base::coast(){
  for(int i = 0; i < 3; i++){
    leftMotors[i].coast();
    rightMotors[i].coast();
  }
}

void Base::brake(){
  for(int i = 0; i < 3; i++){
    leftMotors[i].brake();
    rightMotors[i].brake();
  }
}


void Base::arcToPoint(double itargetX, double itargetY, double itargetA, double maxErrorRadius, bool reverse, int iminSpeed, int imaxSpeed, bool accelerate, bool decelerate){
  targetX = itargetX;
  targetY = itargetY;
  targetA = angleInRange(degToRad(itargetA));

  double totalErrorInitial = findDist(odom.getX(), odom.getY(), targetX, targetY);
  double cordLength = findDist(odom.getX(), odom.getY(), targetX, targetY);
  double cordAngle = findAngle(odom.getX(), odom.getY(), targetX, targetY);
  double radius = (cordLength/2) / (sin((targetA - odom.getA()) / 2));
  double centreX = radius * sin(odom.getA() + degToRad(90)*-boolToSgn(reverse));
  double centreY = radius * cos(odom.getA() + degToRad(90)*-boolToSgn(reverse));
  double radiusToInitialCentre = findDist(odom.getX(), odom.getY(), centreX, centreY);
  double innerRadius = radius - BASE_WIDTH/2;
  double outterRadius = radius + BASE_WIDTH/2;
  double innerRatio = (innerRadius/radius);
  double outterRatio = (outterRadius/radius);
  double pwrY = findPwrY(cordLength, totalErrorInitial, imaxSpeed, iminSpeed, accelerate, decelerate)*-boolToSgn(reverse);
  double pwrA = ((pwrY * outterRatio) - (pwrY / innerRatio));

  unsigned long start = pros::millis();
  unsigned long now = pros::millis();
  unsigned long duration = now - start;

  double minSpeed = iminSpeed / innerRatio;
  double maxSpeed = imaxSpeed / outterRatio;

  double errorAngle = angleInRange(targetA - odom.getA());
  double errorRadius = radiusToInitialCentre - radius;
  double radiusCorrection = 0;

  while((fabs(errorAngle) > 0.00872*4) && (!controller.getPress(X) || controller.getInAutonomous()) && duration < 3000){
    cordLength = findDist(odom.getX(), odom.getY(), targetX, targetY);
    cordAngle = findAngle(odom.getX(), odom.getY(), targetX, targetY);
    radius = (cordLength/2) / (sin((targetA - odom.getA()) / 2));
    innerRadius = radius - (BASE_WIDTH/2);
    outterRadius = radius + (BASE_WIDTH/2);
    innerRatio = (innerRadius/radius);
    outterRatio = (outterRadius/radius);
    minSpeed = iminSpeed / innerRatio;
    maxSpeed = maxSpeed = imaxSpeed / outterRatio;

    radiusToInitialCentre = findDist(odom.getX(), odom.getY(), centreX, centreY);

    pwrY = pwrY = findPwrY(cordLength, totalErrorInitial, imaxSpeed, iminSpeed, accelerate, decelerate)*-boolToSgn(reverse);
    pwrA = (((pwrY * outterRatio) - (pwrY * innerRatio)))*-boolToSgn(reverse);

    errorRadius = radiusToInitialCentre - radius;

    if(errorRadius > maxErrorRadius){
      radiusCorrection = setMin(errorRadius * 3, 20);
    }
    else{
      radiusCorrection = 0;
    }

    pwrA += radiusCorrection*sgn(pwrA);

    errorAngle = angleInRange(targetA - odom.getA());
    setDrive(pwrY, pwrA);
    now = pros::millis();
    duration = now - start;

    pros::delay(DELAY_TIME);
  }
   setDrive(0, 0);
}

//drives to point in main task
void Base::driveToPoint(double itargetX, double itargetY, double maxErrorX, bool reverse, int minSpeed, int maxSpeed, bool accelerate, bool decelerate){//drive to point in the main task
  targetX = itargetX;
  targetY = itargetY;
  double totalErrorInitial = findDist(odom.getX(), odom.getY(), targetX, targetY);
  double totalError = 0;
  double errorAngle = 0;
  double errorX = 0;
  double motionAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY));
  double errorY = totalErrorInitial * cos(errorAngle);
  double totalMotionAngle = 0;
  double pwrX = 0, pwrY = 0;
  double pwrA = 0;
  double totalPwr = 0;

  unsigned long start = pros::millis();
  unsigned long now = pros::millis();
  unsigned long duration = now - start;

  double localYVel = odom.getXVel() * sin(odom.getA()) + odom.getYVel() * cos(odom.getA());

  while(((fabs(errorY) > 2.0625 || localYVel < 20) && (fabs(errorY) > 0.5 || localYVel >= 20)) && (!controller.getPress(X) || controller.getInAutonomous()) && duration < 7000){
    //update errors
    totalError = findDist(odom.getX(), odom.getY(), targetX, targetY);

    //setting velocity
    pwrY = findPwrY(totalError, totalErrorInitial, maxSpeed, minSpeed, accelerate, decelerate);
    motionAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY) + (reverse ? M_PI : 0));
    errorAngle = angleInRange(motionAngle - odom.getA());

    errorX = totalError * sin(errorAngle);
    errorY = totalError * cos(errorAngle);

    if(fabs(errorX) > maxErrorX){
      pwrA = findCorrection(errorX, maxSpeed, minSpeed);
    }
    else{
      pwrA = 0;
    }

    pwrY = pwrY*sgn(cos(errorAngle)) * -boolToSgn(reverse);

    //finding the absolute total of all the values
    totalPwr = fabs(pwrY) + fabs(pwrA);

    //makes sure the power of all the motors is <= 127 to keep the robot moving exactly smoothly
    if(totalPwr > 127){
      pwrY = (pwrY / totalPwr) * 127;
      pwrA = (pwrA / totalPwr) * 127;
    }

    printConsole(pwrY);
    // if(duration > 1000 && safety && odom.getYVel() == 0 && odom.getXVel() == 0 && odom.getAVel() == 0) return;

    // if(pickupMOGOFourBar && fourBar.claw.hasMogo()){
      // motorControl.fourBarGrab();
      // pros::delay(100);
      // break;
    // }
    // pwrY = 20;
    localYVel = odom.getXVel() * sin(odom.getA()) + odom.getYVel() * cos(odom.getA());
    printConsole(localYVel);

    setDrive(pwrY, pwrA);

    now = pros::millis();
    duration = now - start;

    pros::delay(DELAY_TIME);
  }
  // if(odom.getXVel() * sin(odom.getA()) + odom.getYVel() * cos(odom.getA()) > 20){
    while(localYVel > 0){
      localYVel = odom.getXVel() * sin(odom.getA()) + odom.getYVel() * cos(odom.getA());
      setDrive(-10 * -boolToSgn(reverse), 0);
      pros::delay(DELAY_TIME);
    }
  // }
  // else{
  //   while(fabs(errorY) > 0.5){
  //     setDrive(30*fabs(errorY) , 0);
  //     pros::delay(DELAY_TIME);
  //   }
  // }
  setDrive(0, 0);
  printConsole(duration);
}

//drives to point in main task
void Base::driveToDistance(double targetDistance, double itargetA, double maxErrorX, bool reverse, int minSpeed, int maxSpeed, bool accelerate, bool decelerate){//drive to point in the main task
  targetX = targetDistance*sin(itargetA);
  targetY = targetDistance*cos(itargetA);
  double totalErrorInitial = findDist(odom.getX(), odom.getY(), targetX, targetY);
  double totalError = 0;
  double errorAngle = 0;
  double errorX = 0;
  double motionAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY));
  double errorY = totalErrorInitial * cos(errorAngle);
  double totalMotionAngle = 0;
  double pwrX = 0, pwrY = 0;
  double pwrA = 0;
  double totalPwr = 0;

  unsigned long start = pros::millis();
  unsigned long now = pros::millis();
  unsigned long duration = now - start;

  double localYVel = odom.getXVel() * sin(odom.getA()) + odom.getYVel() * cos(odom.getA());

  while(((fabs(errorY) > 2.0625 || localYVel < 20) && (fabs(errorY) > 0.5 || localYVel >= 20)) && (!controller.getPress(X) || controller.getInAutonomous()) && duration < 7000){
    //update errors
    totalError = findDist(odom.getX(), odom.getY(), targetX, targetY);

    //setting velocity
    pwrY = findPwrY(totalError, totalErrorInitial, maxSpeed, minSpeed, accelerate, decelerate);
    motionAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY) + (reverse ? M_PI : 0));
    errorAngle = angleInRange(motionAngle - odom.getA());

    errorX = totalError * sin(errorAngle);
    errorY = totalError * cos(errorAngle);

    if(fabs(errorX) > maxErrorX){
      pwrA = findCorrection(errorX, maxSpeed, minSpeed);
    }
    else{
      pwrA = 0;
    }

    pwrY = pwrY*sgn(cos(errorAngle)) * -boolToSgn(reverse);

    //finding the absolute total of all the values
    totalPwr = fabs(pwrY) + fabs(pwrA);

    //makes sure the power of all the motors is <= 127 to keep the robot moving exactly smoothly
    if(totalPwr > 127){
      pwrY = (pwrY / totalPwr) * 127;
      pwrA = (pwrA / totalPwr) * 127;
    }

    printConsole(pwrY);
    // if(duration > 1000 && safety && odom.getYVel() == 0 && odom.getXVel() == 0 && odom.getAVel() == 0) return;

    // if(pickupMOGOFourBar && fourBar.claw.hasMogo()){
      // motorControl.fourBarGrab();
      // pros::delay(100);
      // break;
    // }
    // pwrY = 20;
    localYVel = odom.getXVel() * sin(odom.getA()) + odom.getYVel() * cos(odom.getA());
    printConsole(localYVel);

    setDrive(pwrY, pwrA);

    now = pros::millis();
    duration = now - start;

    pros::delay(DELAY_TIME);
  }
  // if(odom.getXVel() * sin(odom.getA()) + odom.getYVel() * cos(odom.getA()) > 20){
    while(localYVel > 0){
      localYVel = odom.getXVel() * sin(odom.getA()) + odom.getYVel() * cos(odom.getA());
      setDrive(-10 * -boolToSgn(reverse), 0);
      pros::delay(DELAY_TIME);
    }
  // }
  // else{
  //   while(fabs(errorY) > 0.5){
  //     setDrive(30*fabs(errorY) , 0);
  //     pros::delay(DELAY_TIME);
  //   }
  // }
  setDrive(0, 0);
  printConsole(duration);
}

//drives to point in main task
void Base::turnToPoint(double itargetX, double itargetY, bool reverse, int minSpeed, int maxSpeed, bool accelerate, bool decelerate){//drive to point in the main task
  targetX = itargetX;
  targetY = itargetY;
  double pwrA = 0;
  double errorAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY) - odom.getA() + (reverse ? M_PI : 0));

  unsigned long start = pros::millis();
  unsigned long now = pros::millis();
  unsigned long duration = now - start;


  while(fabs(errorAngle) > degToRad(1) && (!controller.getPress(X) || controller.getInAutonomous()) && duration < 3000){
    errorAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY) - odom.getA() + (reverse ? M_PI : 0));

    pwrA = findPwrA(errorAngle, maxSpeed, minSpeed);

    setDrive(0, pwrA);

    now = pros::millis();
    duration = now - start;

    // if(duration > 1000 && odom.endTarget.getSafety() && velocity.getYVelocity() == 0 && velocity.getXVelocity() == 0 && velocity.getAngleVelocity() == 0) return;

    pros::delay(DELAY_TIME);
  }
  //stopping the drive
  setDrive(0, 0);
}

//drives to point in main task
void Base::turnToAngle(double itargetA, bool reverse, int minSpeed, int maxSpeed, bool accelerate, bool decelerate){//drive to point in the main task
  targetA = angleInRange(degToRad(itargetA));

  double pwrA = 0;
  double errorAngle = angleInRange(targetA - odom.getA() + (reverse ? M_PI : 0));

  unsigned long start = pros::millis();
  unsigned long now = pros::millis();
  unsigned long duration = now - start;


  while(fabs(errorAngle) > degToRad(1) && (!controller.getPress(X) || controller.getInAutonomous()) && duration < 3000){
    errorAngle = angleInRange(targetA - odom.getA() + (reverse ? M_PI : 0));

    pwrA = findPwrA(errorAngle, maxSpeed, minSpeed);

    setDrive(0, pwrA);

    now = pros::millis();
    duration = now - start;

    // if(duration > 1000 && odom.endTarget.getSafety() && velocity.getYVelocity() == 0 && velocity.getXVelocity() == 0 && velocity.getAngleVelocity() == 0) return;

    pros::delay(DELAY_TIME);
  }
  //stopping the drive
  printBrain(4, odom.getA());
  setDrive(0, 0);
}

double Base::findPwrY(double error, double initialError, int maxSpeed, int minSpeed, bool accel, bool decel){
  double accelSlope = 0.0000001;
  double decelSlope = 13.75;
  // if(accel && decel){
    // return setMin(setMax(fabs(127*(tanh(error/decelSlope))-127*tanh((error-initialError)/accelSlope)-127), maxSpeed), minSpeed);
  // }
  // if(accel){
    return setMin(setMax(fabs(127*(tanh(error/decelSlope))), maxSpeed), minSpeed);

  // }
  // if(decel){
    // return setMin(setMax(fabs(-127*(tanh((error-initialError)/decelSlope))), maxSpeed), minSpeed);
  // }
  // return maxSpeed;
}

double Base::findPwrA(double errorAngle, int maxSpeed, int minSpeed){
  double kP = 80;
  double kI = 0;
  double kD = 0;
  // double speed = map_set(fabs(errorAngle),degToRad(0.5), M_PI,30.0*sgn(errorAngle),127.0*sgn(errorAngle),
  //                   degToRad(5), sgn(errorAngle)*10.0,
  //                   degToRad(10), sgn(errorAngle)*15.0,
  //                   degToRad(20),sgn(errorAngle)*30.0,
  //                   degToRad(45),sgn(errorAngle)*40.0,
  //                   degToRad(60), sgn(errorAngle)*50.0,
  //                   degToRad(90),sgn(errorAngle)*80.0,
  //                   M_PI,sgn(errorAngle)*127.0);
  // return sgn(errorAngle)*setMax(setMin(fabs(speed), minSpeed), maxSpeed);
  return 50;
}

double Base::findCorrection(double error, int maxSpeed, int minSpeed){
  double kP = 3;
  double kI = 0;
  double kD = 0;
  double pwr = error * kP;
  return setMin(pwr, 20);
}

void Base::driveToMogo(double mogoX, double mogoY, bool front, double itargetX, double itargetY, double maxErrorX, bool reverse, int minSpeed, int maxSpeed, bool accelerate, bool decelerate){
  targetX = itargetX;
  targetY = itargetY;
  double totalErrorInitial = findDist(odom.getX(), odom.getY(), targetX, targetY);
  double totalError = 0;
  double errorAngle = 0;
  double errorX = 0;
  double motionAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY));
  double errorY = totalErrorInitial * cos(errorAngle);
  double totalMotionAngle = 0;
  double pwrX = 0, pwrY = 0;
  double pwrA = 0;
  double totalPwr = 0;

  unsigned long start = pros::millis();
  unsigned long now = pros::millis();
  unsigned long duration = now - start;

  // int mogosDetected = front ? fourBar.claw.mogoAligned(mogoX, mogoY, front) : transmission.fourBar.claw.mogoAligned(mogoX, mogoY, front);

  while(fabs(errorY) > 0.5 && (!controller.getPress(X) || controller.getInAutonomous()) && duration < 7000){
    //update errors
    totalError = findDist(odom.getX(), odom.getY(), targetX, targetY);

    //setting velocity
    pwrY = findPwrY(totalError, totalErrorInitial, maxSpeed, minSpeed, accelerate, decelerate);
    motionAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY) + (reverse ? M_PI : 0));
    errorAngle = angleInRange(motionAngle - odom.getA());

    errorX = totalError * sin(errorAngle);
    errorY = totalError * cos(errorAngle);

    // mogosDetected = front ? fourBar.claw.mogoAligned(mogoX, mogoY, front) : transmission.fourBar.claw.mogoAligned(mogoX, mogoY, front);

    // if(mogosDetected == 2){
    //   pwrA = 0;
    // }
    // else if(mogosDetected == 0){
    //   if(fabs(errorX) > maxErrorX){
    //     pwrA = findCorrection(errorX, maxSpeed, minSpeed);
    //   }
    // }
    // else{
    //   pwrA = 20 * mogosDetected;
    // }

    pwrY = pwrY*sgn(cos(errorAngle)) * -boolToSgn(reverse);

    //finding the absolute total of all the values
    totalPwr = fabs(pwrY) + fabs(pwrA);

    //makes sure the power of all the motors is <= 127 to keep the robot moving exactly smoothly
    if(totalPwr > 127){
      pwrY = (pwrY / totalPwr) * 127;
      pwrA = (pwrA / totalPwr) * 127;
    }

    // if(duration > 1000 && safety && odom.getYVel() == 0 && odom.getXVel() == 0 && odom.getAVel() == 0) return;

    if(front && fourBar.claw.hasMogo()){
      fourBar.claw.close();
      pros::delay(100);
      break;
    }
    // else if(!front && transmission.fourBar.claw.hasMogo()){
    //   transmission.fourBar.claw.close();
    //   pros::delay(100);
    //   break;
    // }

    setDrive(pwrY, pwrA);

    now = pros::millis();
    duration = now - start;

    pros::delay(DELAY_TIME);
  }
  //stopping the drive
  pros::delay(50);
  setDrive(0, 0);
}
