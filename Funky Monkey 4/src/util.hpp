#include "main.h"
//degree and radian conversion functions
double radToDeg(double rad);
double degToRad(double deg);

//finds the equivilant angle in radians beween 0 and 2pi
double angleInRange(double angle);

//finds the distance between two points
double findDist(const double& x1, const double& y1, const double& x2, const double& y2);
double findAngle(const double& x1, const double& y1, const double& x2, const double& y2);

//find the sign of a number
int sgn(const double& num);

int boolToSgn(bool sgn);
bool sgnToBool(int sgn);

//if the number is > or < (depending on the function) the limit it will set it to the limit
double setMax(double num, const double& limit);
double setMin(double num, const double& limit);

//finding motor velocities
double getActualPwrY();
double getActualPwrA();

double inchesToMilimeters(double inches);
double milimetersToInches(double mm);
