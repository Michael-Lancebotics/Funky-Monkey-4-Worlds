#include "main.h"

class Motor{
private:
  unsigned int port;
  bool reverse;
  int targetSpeed;
  int actualSpeed;


public:
  Motor(){}
  Motor(int iport, bool ireverse){
   port = iport;
   reverse = ireverse;
   targetSpeed = 0;
   actualSpeed = 0;
 }

 int getPort(){
   return port;
 }

 bool getReverse(){
   return reverse;
 }

 void setPort(int iport){
   port = iport;
 }

 void setReverse(int ireverse){
   reverse = ireverse;
 }

void move(int voltage);
void moveVelocity(int rpm);
void moveVoltage(int voltage);
int getTargetSpeed();
int getActualSpeed();
void hold();
void coast();
void brake();
};
