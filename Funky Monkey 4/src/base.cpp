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

double Base::getVelocity(){
  double vel = 0;
  for(int i = 0; i < 3; i++){
    vel += leftMotors[i].getActualSpeed();
    vel += rightMotors[i].getActualSpeed();
  }
  return vel / 6;
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
  double initialRadiusToInitialCentre = findDist(odom.getX(), odom.getY(), centreX, centreY);
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

    errorRadius = initialRadiusToInitialCentre - radiusToInitialCentre;

    if(fabs(errorRadius) > maxErrorRadius){
      radiusCorrection = sgn(errorRadius)*setMin(fabs(errorRadius) * 3, 20);
    }
    else{
      radiusCorrection = 0;
    }

    printConsole(radiusCorrection);
    printConsole(centreX);
    printConsole(centreY);
    printConsole(radiusToInitialCentre);
    printConsole(errorRadius);
    printConsole(radius);
    printConsole(odom.getX());
    printConsole(odom.getY());
    printConsole(radToDeg(odom.getA()));
    printConsole(pwrY);
    printConsole(pwrA);

    // pwrA += radiusCorrection;//*-sgn(pwrA)

    printConsole(pwrA);

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

  while(((fabs(errorY) > 1 || localYVel < 20) && (fabs(errorY) > 0.5 || localYVel >= 20)) && (!controller.getPress(X) || controller.getInAutonomous()) && duration < 7000){
    //update errors
    totalError = findDist(odom.getX(), odom.getY(), targetX, targetY);

    //setting velocity
    pwrY = findPwrY(totalError, totalErrorInitial, maxSpeed, minSpeed, accelerate, decelerate);
    motionAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY) + (reverse ? M_PI : 0));
    errorAngle = angleInRange(motionAngle - odom.getA());

    errorX = totalError * sin(errorAngle);
    errorY = totalError * cos(errorAngle);

    if(fabs(errorX) > maxErrorX){
      pwrA = findCorrection(errorX, minSpeed, maxSpeed);
    }
    else{
      pwrA = 0;
    }
    printConsole(errorX);
    printConsole(radToDeg(odom.getA()));

    pwrY = pwrY*sgn(cos(errorAngle)) * -boolToSgn(reverse);

    if(!decelerate){
      pwrY = maxSpeed*sgn(pwrY);
    }

    //finding the absolute total of all the values
    totalPwr = fabs(pwrY) + fabs(pwrA);

    //makes sure the power of all the motors is <= 127 to keep the robot moving exactly smoothly
    if(totalPwr > 127){
      pwrY = (pwrY / totalPwr) * 127;
      pwrA = (pwrA / totalPwr) * 127;
    }

    printConsole(pwrY);
    printConsole(pwrA);
    // if(duration > 1000 && safety && odom.getYVel() == 0 && odom.getXVel() == 0 && odom.getAVel() == 0) return;

    // if(pickupMOGOFourBar && fourBar.claw.hasMogo()){
      // motorControl.fourBarGrab();
      // pros::delay(100);
      // break;
    // }
    // pwrY = 20;
    localYVel = odom.getXVel() * sin(odom.getA()) + odom.getYVel() * cos(odom.getA());
    // printConsole(localYVel);

    setDrive(pwrY, pwrA);

    now = pros::millis();
    duration = now - start;

    printConsole(getVelocity());
    printConsole(duration);
    pros::delay(DELAY_TIME);
  }
  // if(odom.getXVel() * sin(odom.getA()) + odom.getYVel() * cos(odom.getA()) > 20){
    // while(localYVel*-boolToSgn(reverse) > 0){
    //   localYVel = odom.getXVel() * sin(odom.getA()) + odom.getYVel() * cos(odom.getA());
    //   setDrive(-10 * -boolToSgn(reverse), 0);
    //   pros::delay(1);
    // }
  // }
  // else{
  //   while(fabs(errorY) > 0.5){
  //     setDrive(30*fabs(errorY) , 0);
  //     pros::delay(DELAY_TIME);
  //   }
  // }
  now = pros::millis();
  duration = now - start;
  if(decelerate){
    setDrive(0, 0);
  }
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
    pwrY = findPwrY(totalError, totalErrorInitial, minSpeed, maxSpeed, accelerate, decelerate);
    motionAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY) + (reverse ? M_PI : 0));
    errorAngle = angleInRange(motionAngle - odom.getA());

    errorX = totalError * sin(errorAngle);
    errorY = totalError * cos(errorAngle);

    if(fabs(errorX) > maxErrorX){
      pwrA = findCorrection(errorX, minSpeed, maxSpeed);
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

  double initError = radToDeg(errorAngle);

  while(fabs(errorAngle) > degToRad(1) && (!controller.getPress(X) || controller.getInAutonomous()) && duration < 3000){
    errorAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY) - odom.getA() + (reverse ? M_PI : 0));

    pwrA = findPwrA(errorAngle, minSpeed, maxSpeed, initError)*sgn(errorAngle);

    printConsole(radToDeg(errorAngle));
    printConsole(pwrA);
    setDrive(0, pwrA);
    printConsole(getActualPwrA());

    now = pros::millis();
    duration = now - start;
    printConsole(duration);

    // if(duration > 1000 && odom.endTarget.getSafety() && velocity.getYVelocity() == 0 && velocity.getXVelocity() == 0 && velocity.getAngleVelocity() == 0) return;

    pros::delay(DELAY_TIME);
  }
  // while(odom.getAVel() * -boolToSgn(reverse) > 0){
  //   setDrive(0,-10 * -boolToSgn(reverse));
  //   pros::delay(DELAY_TIME);
  // }
  //stopping the drive
  setDrive(0, 0);
  now = pros::millis();
  duration = now - start;
  printConsole(duration);

}

//drives to point in main task
void Base::turnToAngle(double itargetA, bool reverse, int minSpeed, int maxSpeed, bool accelerate, bool decelerate){//drive to point in the main task
  targetA = angleInRange(degToRad(itargetA));

  double pwrA = 0;
  double errorAngle = angleInRange(targetA - odom.getA() + (reverse ? M_PI : 0));

  unsigned long start = pros::millis();
  unsigned long now = pros::millis();
  unsigned long duration = now - start;

  double initError = radToDeg(errorAngle);

  while(fabs(errorAngle) > degToRad(1) && (!controller.getPress(X) || controller.getInAutonomous()) && duration < 3000){
    errorAngle = angleInRange(targetA - odom.getA() + (reverse ? M_PI : 0));
    pwrA = setMin(setMax(findPwrA(errorAngle, minSpeed, maxSpeed, initError), maxSpeed), minSpeed)*sgn(errorAngle);

    printConsole(radToDeg(errorAngle));
    printConsole(pwrA);
    setDrive(0, pwrA);
    printConsole(getActualPwrA());

    now = pros::millis();
    duration = now - start;
    printConsole(duration);

    // if(duration > 1000 && odom.endTarget.getSafety() && velocity.getYVelocity() == 0 && velocity.getXVelocity() == 0 && velocity.getAngleVelocity() == 0) return;

    pros::delay(DELAY_TIME);
  }
  // while(odom.getAVel() * -boolToSgn(reverse) > 0){
  //   setDrive(0,-10 * -boolToSgn(reverse));
  //   pros::delay(DELAY_TIME);
  // }
  //stopping the drive
  setDrive(0, 0);
  now = pros::millis();
  duration = now - start;
  printConsole(duration);
}

double Base::findPwrY(double error, double initialError, int maxSpeed, int minSpeed, bool accel, bool decel){
  // double accelSlope = 0.0000001;
  double decelSlope = 16;
  // if(accel && decel){
    // return setMin(setMax(fabs(127*(tanh(error/decelSlope))-127*tanh((error-initialError)/accelSlope)-127), maxSpeed), minSpeed);
  // }
  // if(accel){
    return setMin(setMax(fabs(130*(tanh(error/decelSlope))), maxSpeed), minSpeed);

  // }
  // if(decel){
    // return setMin(setMax(fabs(-127*(tanh((error-initialError)/decelSlope))), maxSpeed), minSpeed);
  // }
  // return maxSpeed;
}

double Base::findPwrA(double errorAngle, int maxSpeed, int minSpeed, double initError){
  double kP = 80;
  double kI = 0;
  double kD = 0;
  int profile = round(initError/10)*10;
  printConsole(initError);
  printConsole(profile);
  if(radToDeg(fabs(errorAngle)) > 90){
    return 127;
  }
  else if(radToDeg(fabs(errorAngle)) > 80){
    return 127;
  }
  else if(radToDeg(fabs(errorAngle)) > 70){
    return 127;
  }
  else if(radToDeg(fabs(errorAngle)) > 60){
    return 127;
  }
  else if(radToDeg(fabs(errorAngle)) > 50){
    return profile <= 70 ? 100 : 80;
  }
  else if(radToDeg(fabs(errorAngle)) > 40){
    return profile <= 50 ? 80 : 40;
  }
  else if(radToDeg(fabs(errorAngle)) > 30){
    return profile == 80 ? 40 : profile <= 70 ? 40 : 30;
  }
  else if(radToDeg(fabs(errorAngle)) > 20){
    return profile <= 80 && profile >= 60 ? 40 : profile == 50 ? 30 : profile <= 40 ? 45 : 20;
  }
  else if(radToDeg(fabs(errorAngle)) > 10){
    return 20;
  }
  else{
    return 20;
  }
}

double Base::findCorrection(double error, int maxSpeed, int minSpeed){
  double kP = 3;
  double kI = 0;
  double kD = 0;
  double pwr = error * kP;
  return sgn(pwr)*setMin(fabs(pwr), 20);
}

void Base::driveToMogo(double mogoX, double mogoY, bool correct, double itargetX, double itargetY, double maxErrorX, bool reverse, int minSpeed, int maxSpeed, bool accelerate, bool decelerate, bool goFast){
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

  int mogosDetected = reverse ? twoBar.mogoAligned(mogoX, mogoY) : fourBar.claw.mogoAligned(mogoX, mogoY);

  while(fabs(errorY) > 0.5 && (!controller.getPress(X) || controller.getInAutonomous()) && duration < 7000){
    //update errors
    totalError = findDist(odom.getX(), odom.getY(), targetX, targetY);

    //setting velocity
    pwrY = findPwrY(totalError, totalErrorInitial, minSpeed, maxSpeed, accelerate, decelerate);
    motionAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY) + (reverse ? M_PI : 0));
    errorAngle = angleInRange(motionAngle - odom.getA());

    errorX = totalError * sin(errorAngle);
    errorY = totalError * cos(errorAngle);

    mogosDetected = reverse ? twoBar.mogoAligned(mogoX, mogoY) : fourBar.claw.mogoAligned(mogoX, mogoY);

    if(correct){
      if(mogosDetected == 2){
        pwrA = 0;
      }
      else if(mogosDetected == 0){
        if(fabs(errorX) > maxErrorX){
          pwrA = findCorrection(errorX, minSpeed, maxSpeed);
        }
      }
      else{
        pwrA = 20 * mogosDetected * -boolToSgn(reverse);
      }
    }
    else{
      if(fabs(errorX) > maxErrorX){
        pwrA = findCorrection(errorX, minSpeed, maxSpeed);
      }
    }

    pwrY = pwrY*sgn(cos(errorAngle)) * -boolToSgn(reverse);

    //finding the absolute total of all the values
    totalPwr = fabs(pwrY) + fabs(pwrA);

    //makes sure the power of all the motors is <= 127 to keep the robot moving exactly smoothly
    if(totalPwr > 127){
      pwrY = (pwrY / totalPwr) * 127;
      pwrA = (pwrA / totalPwr) * 127;
    }

    localYVel = odom.getXVel() * sin(odom.getA()) + odom.getYVel() * cos(odom.getA());

    if(!reverse && (((fourBar.claw.getDistance() < 172 && fourBar.claw.getDistance() != 0) && goFast) || (fourBar.claw.hasMogo() && !goFast))){
      fourBar.claw.close();
      printBrain(6, fourBar.claw.getDistance());
      printBrain(7, duration);
      // setDrive(127, 0);
      pros::delay(50);
      break;
    }
    else if(reverse && twoBar.hasMogo()){
      // pros::delay(100);
      // twoBar.close();
      break;
    }

    setDrive(pwrY, pwrA);

    now = pros::millis();
    duration = now - start;

    pros::delay(DELAY_TIME);
  }
  //stopping the drive
  setDrive(0, 0);
}

void Base::turnToMogo(double mogoX, double mogoY, bool reverse, int minSpeed, int maxSpeed, bool accelerate, bool decelerate){
  targetX = mogoX;
  targetY = mogoY;
  double pwrA = 0;
  double errorAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY) - odom.getA() + (reverse ? M_PI : 0));
  errorAngle += degToRad(15)*sgn(errorAngle);

  unsigned long start = pros::millis();
  unsigned long now = pros::millis();
  unsigned long duration = now - start;

  double initError = radToDeg(errorAngle);

  // bool left = (errorAngle > 0 && !reverse) || (errorAngle > 0 && reverse);

  int mogosDetected = reverse ? twoBar.mogoAligned(mogoX, mogoY) : fourBar.claw.mogoAligned(mogoX, mogoY);

  while(mogosDetected != 2 && (!controller.getPress(X) || controller.getInAutonomous()) && duration < 3000){
    errorAngle = angleInRange(findAngle(odom.getX(), odom.getY(), targetX, targetY) - odom.getA() + (reverse ? M_PI : 0));
    errorAngle += degToRad(15)*sgn(errorAngle);

    pwrA = findPwrA(errorAngle, minSpeed, maxSpeed, initError)*sgn(errorAngle);

    if(fabs(errorAngle) < degToRad(15)){
      pwrA = 15*sgn(pwrA);
    }

    setDrive(0, pwrA);

    now = pros::millis();
    duration = now - start;
    printConsole(duration);

    // if(duration > 1000 && odom.endTarget.getSafety() && velocity.getYVelocity() == 0 && velocity.getXVelocity() == 0 && velocity.getAngleVelocity() == 0) return;
    mogosDetected = reverse ? twoBar.mogoAligned(mogoX, mogoY) : fourBar.claw.mogoAligned(mogoX, mogoY);
    printConsole(mogosDetected);
    pros::delay(DELAY_TIME);
  }
  // while(odom.getAVel() * -boolToSgn(reverse) > 0){
  //   setDrive(0,-10 * -boolToSgn(reverse));
  //   pros::delay(DELAY_TIME);
  // }
  //stopping the drive
  setDrive(0, 0);
  now = pros::millis();
  duration = now - start;
  printConsole(duration);
}

double Base::getActualPwrA(){
  double pwrA = 0;
  for(int i = 0; i < 3; i++){
    pwrA += leftMotors[i].getActualSpeed();
    pwrA -= rightMotors[i].getActualSpeed();
  }
  return pwrA/3;
}
