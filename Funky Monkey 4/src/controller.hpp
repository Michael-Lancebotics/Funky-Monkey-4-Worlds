#include "main.h"

#define UP DIGITAL_UP
#define DOWN DIGITAL_DOWN
#define LEFT DIGITAL_LEFT
#define RIGHT DIGITAL_RIGHT

#define A DIGITAL_A
#define B DIGITAL_B
#define X DIGITAL_X
#define Y DIGITAL_Y

#define R1 DIGITAL_R1
#define R2 DIGITAL_R2
#define L1 DIGITAL_L1
#define L2 DIGITAL_L2

#define LX ANALOG_LEFT_X
#define LY ANALOG_LEFT_Y
#define RX ANALOG_RIGHT_X
#define RY ANALOG_RIGHT_Y

class Controller{
private:
  pros::Controller controller = pros::Controller(pros::E_CONTROLLER_MASTER);
public:
  Controller(){}

  bool getPress(pros::controller_digital_e_t button){
    return controller.get_digital(button);
  }

  bool getNewPress(pros::controller_digital_e_t button){
    return controller.get_digital_new_press(button);
  }

  short getJoystick(pros::controller_analog_e_t joystick){
    return controller.get_analog(joystick);
  }

  short getBattery(){
    return controller.get_battery_level();
  }

  bool getCompetitionSwitch(){
    return pros::competition::is_connected();
  }

  bool getInAutonomous(){
    return pros::competition::is_autonomous();
  }

  bool getEnabled(){
    return !pros::competition::is_disabled();
  }

  void rumble(const char *pattern){
    controller.rumble(pattern);
  }
};
