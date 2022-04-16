#include "rollers.hpp"
#include "button.hpp"

enum class TransmissionState{ fourBar, rollers };
class Transmission{
private:
  Piston piston;
  TransmissionState state;
  Button bottom;

public:
  Transmission(){}
  Transmission(Piston ipiston, FourBar ifourBar, Rollers irollers, Button ibottom){
    piston = ipiston;
    fourBar.equals(ifourBar);
    rollers = irollers;
    bottom = ibottom;
  }

  FourBar fourBar;
  Rollers rollers;
  void setState(TransmissionState istate){
    std::cout << "???" << std::endl;
    state = istate;
    rollers.move(20);
    pros::delay(50);
    piston.setState(static_cast<PistonState>(state));
    pros::delay(50);
    rollers.move(0);
  }

  TransmissionState getState(){
    return state;
  }

  void hold(){
    fourBar.hold();
  }

  bool ringable();
  void runFourBar();
  void runConveyors();
  void run();
  void calibrate();
};
