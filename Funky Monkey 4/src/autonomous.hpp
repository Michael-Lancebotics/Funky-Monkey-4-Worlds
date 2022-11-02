#include "main.h"
#include "button.hpp"

class Auton{
private:
  static void rightSide();
  static void middle();
  static void leftSide();
  static void winPoint();
  static void rightMiddle();
  pros::Task *fourBarTask = nullptr;
  pros::Task *rollerTask = nullptr;

  static void fourBarController();
  static void rollerController();

  Button selector;

  int auton;
  std::vector<void(*)()> autons;
  std::vector<std::string> autonNames;

public:
  Auton(Button iselector){
    selector = iselector;
    autons = {rightSide, leftSide, middle, winPoint, rightMiddle};
    autonNames = {"rightSide", "leftSide", "middle", "winPoint", "rightMiddle"};
  }

  void programmingSkills();

  void setAuton(int iauton){
    auton = iauton;
  }

  void programmingSkillsSetup();
  void start();
  void select();
  void stop();
};
