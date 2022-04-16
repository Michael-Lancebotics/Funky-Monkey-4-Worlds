#include "robot.hpp"

void Odometry::start(){//starts a new task for robot
  updateTask = new pros::Task(update);
}

void Odometry::stop(){//stops the robot task
  if(updateTask != nullptr){
    updateTask->remove();
    delete updateTask;
    updateTask = nullptr;
  }
}

void Odometry::startDistanceFilter(){//starts a new task for robot
  distanceFilterTask = new pros::Task(distanceFilter);
}

void Odometry::stopDistanceFilter(){//stops the robot task
  if(distanceFilterTask != nullptr){
    distanceFilterTask->remove();
    delete distanceFilterTask;
    distanceFilterTask = nullptr;
  }
}

void Odometry::reset(double ix, double iy, double iangle){
  odom.setAngle(degToRad(iangle));
  odom.inertial.setRotation(iangle);
  odom.setX(ix);
  odom.setY(iy);
  // position.setX(ix);
  // position.setY(iy);
  // position.setAngle(iangle);
}

void Odometry::reset(double ix, double iy){
  odom.setX(ix);
  odom.setY(iy);
  // position.setX(ix);
  // position.setY(iy);
}

void Odometry::reset(){
  odom.setAngle(0);
  odom.inertial.setRotation(0);
  odom.setX(0);
  odom.setY(0);
  // position.setX(0);
  // position.setY(0);
  // position.setAngle(0);
}

void Odometry::calibrate(){
  stop();
  stopDistanceFilter();
  startDistanceFilter();
  reset();
  inertial.calibrate();
  start();
}

void Odometry::wallReset(bool front){
  stop();
  double angle;
  bool useLeft = (left.getDistance() < right.getDistance() && left.getDistance() > 0) || right.getDistance() == 0;
  double localX = milimetersToInches(useLeft ? left.getFilteredDistance() : right.getFilteredDistance());
  double localXOffset = useLeft ? 7.125 : -7.125;
  double localYOffset = 7.5;
  if(angleInRange(a) > degToRad(-45) && angleInRange(a) < degToRad(45)){
    angle = degToRad(0);
    reset((front == useLeft ? localX : 143 - localX) + (front ? localXOffset : -localXOffset), (front ? 0 + 143 - localYOffset : localYOffset), angle);
  }
  else if(angleInRange(a) > degToRad(45) && angleInRange(a) < degToRad(135)){
    angle = degToRad(90);
    reset((front ? 0 + 143 - localYOffset : localYOffset), (front == useLeft ? localX : 143 - localX) + (front ? localXOffset : -localXOffset), angle);
  }
  else if(angleInRange(a) > degToRad(135) && angleInRange(a) < degToRad(-135)){
    angle = degToRad(180);
    reset((front == useLeft ? localX : 143 - localX) + (front ? localXOffset : -localXOffset), (front ? 0 + 143 - localYOffset : localYOffset), angle);
  }
  else{
    angle = degToRad(-90);
    reset((front ? 0 + 143 - localYOffset : localYOffset), (front == useLeft ? localX : 143 - localX) + (front ? localXOffset : -localXOffset), angle);
  }
  start();
}

void Odometry::fullReset(){
  stop();
  bool useLeft = (left.getDistance() < right.getDistance() && left.getDistance() > 0) || right.getDistance() == 0;
  double lrDist = 14.25;
  double lfDist = milimetersToInches(frontLeft.getFilteredDistance());
  double rfDist = milimetersToInches(frontRight.getFilteredDistance());
  double avgDist = (lfDist + rfDist) / 2;
  double theta = 0;
  double xPosition = 0;
  double yPosition = 0;
  double angle = 0;
  double localXOffset = useLeft ? 7.125 : -7.125;
  double localYOffset = 1.5;
  printConsole(left.getDistance());
  printConsole(right.getDistance());
  printConsole(useLeft);
  printConsole(lfDist);
  printConsole(rfDist);
  printConsole(avgDist);
  if(angleInRange(a) > degToRad(-45) && angleInRange(a) < degToRad(45)){
    angle = atan(lrDist/(rfDist - lfDist)) + degToRad(0);
    theta = degToRad(90) - angle;
    xPosition = cos(theta) * milimetersToInches(useLeft ? left.getFilteredDistance() + localXOffset : 143 - (right.getFilteredDistance() + localXOffset));
    yPosition = cos(theta) * (143 - (avgDist + localYOffset));
  }
  else if(angleInRange(a) > degToRad(45) && angleInRange(a) < degToRad(135)){
    printConsoleText("2");
    angle = tan(lrDist/(lfDist - rfDist)) + degToRad(90);
    theta = degToRad(90) - (degToRad(90) - angle);
    xPosition = cos(theta) * (143 - (avgDist + localYOffset));
    yPosition = cos(theta) * milimetersToInches(useLeft ? 143 - (left.getFilteredDistance() + localXOffset) : right.getFilteredDistance() + localXOffset);
  }
  else if(angleInRange(a) > degToRad(135) && angleInRange(a) < degToRad(-135)){
    printConsoleText("3");
    angle = tan(lrDist/(lfDist - rfDist)) + degToRad(180);
    theta = degToRad(90) - (degToRad(180) - angle);
    xPosition = cos(theta) * milimetersToInches(useLeft ? 143 - (left.getFilteredDistance() + localXOffset) : right.getFilteredDistance() + localXOffset);
    yPosition = cos(theta) * (avgDist + localYOffset);
  }
  else{
    printConsoleText("4");
    angle = tan(lrDist/(lfDist - rfDist)) + degToRad(-90);
    theta = degToRad(90) - (degToRad(-90) - angle);
    xPosition = cos(theta) * (avgDist + localYOffset);
    yPosition = cos(theta) * milimetersToInches(useLeft ? left.getFilteredDistance() + localXOffset : 143 - (right.getFilteredDistance() + localXOffset));
  }
  reset(xPosition, yPosition, angle);
  start();
}

void Odometry::update(){//odometry
  double rightRadius = 0, sideRadius = 0;
  double rightCord = 0;
  double sideCord = 0;
  double alpha = 0, averageTheta = 0;
  double deltaLocalXSide = 0, deltaLocalYSide = 0, deltaLocalXRight = 0, deltaLocalYRight = 0;
  double currentLeft = 0, currentRight = 0, currentSide = 0;
  double deltaLeft = 0, deltaRight = 0, deltaSide = 0;
  double lastLeft = 0, lastRight = 0, lastSide = 0;
  unsigned long lastCheck = 0;
  double lastX = 0, lastY = 0, lastAngle = 0;
  double inertialValue = 0, angleVal = 0;
  double deltaTheta = 0;
  double currentRotations = 0;
  double lastRotations = 0;
  double currentPitch = 0;
  double lastPitch = 0;
  double currentRoll = 0;
  double lastRoll = 0;
  while (true){
	  currentRight = odom.yWheel.getPosition() / 360.0 * (odom.yWheel.getDiameter() * M_PI);
	  currentSide = odom.xWheel.getPosition() / 360.0 * (odom.xWheel.getDiameter() * M_PI);

    deltaSide = currentSide - lastSide;
	  deltaRight = currentRight - lastRight;

    lastSide = currentSide;
	  lastRight = currentRight;

    inertialValue = degToRad(odom.inertial.getRotation());
    deltaTheta = inertialValue - odom.a;

	  if (deltaTheta != 0) {
      alpha = deltaTheta / 2.0;

		  rightRadius = deltaRight / deltaTheta + odom.yWheel.getDistance();
		  rightCord = (rightRadius * sin(alpha)) * 2;

		  sideRadius = deltaSide / deltaTheta + odom.xWheel.getDistance();
		  sideCord = (sideRadius * sin(alpha)) * 2;
	  }

	  else {
		  rightCord = deltaRight;
		  sideCord = deltaSide;
		  alpha = 0.0;
	  }

	  averageTheta = odom.a + alpha;

    odom.x += rightCord * sin(averageTheta);
    odom.y += rightCord * cos(averageTheta);

    odom.x += sideCord * cos(averageTheta);
    odom.y += sideCord * -sin(averageTheta);

    //updating the global angle
	  odom.a += deltaTheta;

    // position.setX(odom.x);
    // position.setY(odom.y);
    // position.setAngle(odom.a);

    if(pros::millis() >= lastCheck + 40){
      odom.velX = ((odom.x - lastX) * 1000) / (pros::millis() - lastCheck);
      odom.velY = ((odom.y - lastY) * 1000) / (pros::millis() - lastCheck);
      odom.velA = ((odom.a - lastAngle) * 1000) / (pros::millis() - lastCheck);
      lastX = odom.x;
      lastY = odom.y;
      lastAngle = odom.a;
      lastCheck = pros::millis();
    }

    // velocity.setvelX(odom.velX);
    // velocity.setvelY(odom.velY);
    // velocity.setvelA(odom.velA);

    pros::lcd::clear_line(1);
    pros::lcd::print(1, "x: %.2f y: %.2f, a: %.2f", odom.x, odom.y, radToDeg(odom.a));
	  pros::delay(DELAY_TIME);
  }
}

void Odometry::distanceFilter(){
  pros::delay(50);
  int ltemp;
  int rtemp;
  int lftemp;
  int rftemp;
  int lvalues[100];
  int rvalues[100];
  int lfvalues[100];
  int rfvalues[100];
  for(int i = 0; i < 100; i++){
    lvalues[i] = odom.left.getDistance();
    rvalues[i] = odom.right.getDistance();
    lfvalues[i] = odom.frontLeft.getDistance();
    rfvalues[i] = odom.frontRight.getDistance();
    pros::delay(10);
  }
  while(true){
    ltemp = 0;
    rtemp = 0;
    lftemp = 0;
    rftemp = 0;
    for(int i = 0; i < 99; i++){
      lvalues[i] = lvalues[i+1];
      rvalues[i] = rvalues[i+1];
      lfvalues[i] = lfvalues[i+1];
      rfvalues[i] = rfvalues[i+1];
      ltemp += lvalues[i];
      rtemp += rvalues[i];
      lftemp += lfvalues[i];
      rftemp += rfvalues[i];
    }
    lvalues[99] = odom.left.getDistance();
    rvalues[99] = odom.right.getDistance();
    lfvalues[99] = odom.frontLeft.getDistance();
    rfvalues[99] = odom.frontRight.getDistance();
    ltemp += lvalues[99];
    rtemp += rvalues[99];
    lftemp += lfvalues[99];
    rftemp += rfvalues[99];
    ltemp /= 100;
    rtemp /= 100;
    lftemp /= 100;
    rftemp /= 100;
    odom.left.setFilteredDistance(round(ltemp));
    odom.right.setFilteredDistance(round(rtemp));
    odom.frontLeft.setFilteredDistance(round(lftemp));
    odom.frontRight.setFilteredDistance(round(rftemp));
    pros::delay(10);
  }
}
