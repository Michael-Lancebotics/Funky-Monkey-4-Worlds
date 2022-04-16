#include "main.h"

enum class PistonState{ retract, extend };
class Piston{
private:
  int port;
  bool reverse;
  PistonState state;
  pros::ADIDigitalOut *piston;

public:
  Piston(){}
  Piston(int iport, bool ireverse){
    port = iport;
    reverse = ireverse;
    piston = new pros::ADIDigitalOut(port);
  }

  int getPort(){
    return port;
  }

  bool getReverse(){
    return reverse;
  }

  PistonState getState(){
    return state;
  }

  void setPort(int iport){
    port = iport;
  }

  void setReverse(int ireverse){
    reverse = ireverse;
  }

  void setState(PistonState istate);
  void switchState();
  void extend();
  void retract();
};
