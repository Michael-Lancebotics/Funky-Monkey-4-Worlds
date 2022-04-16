#include "main.h"

class DriveControl{
private:
  pros::Task *baseTask = nullptr;
  pros::Task *fourBarTask = nullptr;
  pros::Task *fourBarManagerTask = nullptr;
  pros::Task *rollerTask = nullptr;
  pros::Task *twoBarTask = nullptr;

  static void baseController();
  static void fourBarController();
  static void fourBarManager();
  static void twoBarController();
  static void rollerController();

public:
  void start();
  void stop();
};
