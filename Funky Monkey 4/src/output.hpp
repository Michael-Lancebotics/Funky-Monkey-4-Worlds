#include "main.h"

#define printConsole(x) std::cout << #x << ": " <<  x << std::endl
#define printConsoleText(x) std::cout << x << std::endl
#define printBrain(line, x) pros::lcd::clear_line(line); pros::lcd::set_text(line, std::string(#x) + ": " + std::to_string(x))
#define printBrainText(line, x) pros::lcd::clear_line(line); pros::lcd::set_text(line, x)