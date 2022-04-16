#include "main.h"

class DriveControl{
private:
  pros::Task *baseTask = nullptr;
  pros::Task *fourBarTask = nullptr;
  pros::Task *fourBarManagerTask = nullptr;
  pros::Task *transmissionManagerTask = nullptr;
  pros::Task *transmissionTask = nullptr;

  static void baseController();
  static void fourBarController();
  static void fourBarManager();
  static void transmissionController();
  static void transmissionManager();

public:
  void start();
  void stop();
};
