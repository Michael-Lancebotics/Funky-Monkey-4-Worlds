#include "main.h"

class Auton{
private:
  void programmingSkills();
  void rightSide();
  void middleFromRight();
  void middleFromLeft();
  void leftSide();
  void winPoint();
  pros::Task *fourBarTask = nullptr;
  pros::Task *rollerTask = nullptr;
  pros::Task *testTask = nullptr;

  static void fourBarController();
  static void rollerController();
  static void test();

public:
  Auton(){}
  void programmingSkillsSetup();
  void start();
  void select();
};
