#include "main.h"
#include <fstream>

#define printText(line, x) pros::lcd::clear_line(line); pros::lcd::set_text(line, std::string(x))
#define printVar(line, x) pros::lcd::clear_line(line); pros::lcd::set_text(line, std::string(#x) + ": " + std::to_string(x))
#define printConsole(x) std::cout << #x << ": " <<  x << std::endl

#define GETJOY master.get_analog
#define GETBUTTON master.get_digital
#define GETNEWPRESS master.get_digital_new_press

#define LX ANALOG_LEFT_X
#define LY ANALOG_LEFT_Y
#define RX ANALOG_RIGHT_X
#define RY ANALOG_RIGHT_Y

//button macros
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

pros::Controller master(pros::E_CONTROLLER_MASTER);

pros::Vision TwoBarMOGOAligner(11);
pros::Vision FourBarMOGOAligner(14);

pros::Distance WallSquareLeft(3);
pros::Distance WallSquareRight(21);
pros::Distance WallSquareSide(18);
pros::Distance FourBarMOGODetect(8);
pros::Distance TwoBarMOGODetect(17);

pros::Rotation TwoBarRotation(8, true);
pros::Rotation FourBarRotation(2, true);
//
// pros::Vision FourBarAligner(2);
//2 is 3 wire extender
pros::Imu Inertial1(13);
pros::Imu Inertial2(22);

pros::Motor LeftF(1, true);
pros::Motor LeftM(4, true);
pros::Motor LeftB(12, true);
pros::Motor RightF(5);
pros::Motor RightM(21);
pros::Motor RightB(16);
pros::Motor FourBar(20);
pros::Motor Rollers(10);

pros::ADIDigitalOut TwoBarPiston(3);
pros::ADIDigitalOut FourBarPiston(4);

pros::ADIEncoder EncoderS(7, 8);
pros::ADIEncoder EncoderR(1, 2);

void initialize() {
	pros::lcd::initialize();
}

void clearScreen(){
	for(int i = 0; i < 8; i++){
		pros::lcd::clear_line(i);
	}
}

void fourBar(){
	while(!GETNEWPRESS(L2)){
		clearScreen();
		printText(0, "4-BAR    |    PRESS L2 TO RETURN");
		printText(1, "Press A to check motor temp");
		printText(2, "Press B to tune hover height");
		printText(3, "Press X to tune score height");
		printText(4, "Press R2 to tune down height");
		printText(5, "Press Y to check for stalling");
		if(GETNEWPRESS(A)){
			while(!GETNEWPRESS(L2)){
				clearScreen();
				printText(0, "4-BAR    |    PRESS L2 TO RETURN");
				printVar(1, FourBar.get_temperature());
				printVar(2, FourBar.get_temperature());
				pros::delay(10);
			}
		}
		if(GETNEWPRESS(B)){
			int previousValue = 0;
			std::string line;
  		std::ifstream fourBarHoverRead ("/usd/Maintanence/Four Bar Hover.txt");
  		if (fourBarHoverRead.is_open()){
    		while ( getline (fourBarHoverRead,line) ){
      		previousValue = std::stoi(line);
				}
    	fourBarHoverRead.close();
  		}
			int currentValue = FourBarRotation.get_position();
			while(!GETNEWPRESS(L2)){//add a warning if not saved
				clearScreen();
				currentValue = FourBarRotation.get_position();
				printText(0, "4-BAR HOVER | PRESS L2 TO RETURN");
				printText(1, "Press A to save");
				printText(2, "Press R2 to see previous value");
				printText(3, "Control the 4-bar on the left");
				printText(4, "joystick y axis");
				printVar(5, previousValue);
				printVar(6, currentValue);
				FourBar.move(GETJOY(LY));
				FourBar.move(GETJOY(LY));
				pros::delay(10);
				if(GETNEWPRESS(A)){
					std::ofstream fourBarHoverWrite;
				  fourBarHoverWrite.open("/usd/Maintanence/Four Bar Hover.txt");
				  fourBarHoverWrite << currentValue;
				  fourBarHoverWrite.close();
					std::string line;
		  		fourBarHoverRead.open("/usd/Maintanence/Four Bar Hover.txt");
		  		if (fourBarHoverRead.is_open()){
		    		while ( getline (fourBarHoverRead,line) ){
		      		previousValue = std::stoi(line);
						}
		    	fourBarHoverRead.close();
		  		}
				}
			}
		}
		else if(GETNEWPRESS(X)){
			int previousValue = 0;
			std::string line;
  		std::ifstream fourBarScoreRead ("/usd/Maintanence/Four Bar Score.txt");
  		if (fourBarScoreRead.is_open()){
    		while ( getline (fourBarScoreRead,line) ){
      		previousValue = std::stoi(line);
				}
    	fourBarScoreRead.close();
  		}
			int currentValue = FourBarRotation.get_position();
			while(!GETNEWPRESS(L2)){//add a warning if not saved
				clearScreen();
				printConsole(FourBarRotation.get_angle());

				currentValue = FourBarRotation.get_position();
				printText(0, "4-BAR SCORE | PRESS L2 TO RETURN");
				printText(1, "Press A to save");
				printText(2, "Press R2 to see previous value");
				printText(3, "Control the 4-bar on the left");
				printText(4, "joystick y axis");
				printVar(5, previousValue);
				printVar(6, currentValue);
				FourBar.move(GETJOY(LY));
				FourBar.move(GETJOY(LY));
				pros::delay(10);
				if(GETNEWPRESS(A)){
					std::ofstream fourBarScoreWrite;
				  fourBarScoreWrite.open("/usd/Maintanence/Four Bar Score.txt");
				  fourBarScoreWrite << currentValue;
				  fourBarScoreWrite.close();
					std::string line;
		  		fourBarScoreRead.open("/usd/Maintanence/Four Bar Score.txt");
		  		if (fourBarScoreRead.is_open()){
		    		while ( getline (fourBarScoreRead,line) ){
		      		previousValue = std::stoi(line);
						}
		    	fourBarScoreRead.close();
		  		}
				}
			}
		}
		else if(GETNEWPRESS(R2)){
			int previousValue = 0;
			std::string line;
  		std::ifstream fourBarDownRead ("/usd/Maintanence/Four Bar Down.txt");
  		if (fourBarDownRead.is_open()){
    		while ( getline (fourBarDownRead,line) ){
      		previousValue = std::stoi(line);
				}
    	fourBarDownRead.close();
  		}
			int currentValue = FourBarRotation.get_position();
			while(!GETNEWPRESS(L2)){//add a warning if not saved
				clearScreen();
				printConsole(FourBarRotation.get_angle());

				currentValue = FourBarRotation.get_position();
				printText(0, "4-BAR SCORE | PRESS L2 TO RETURN");
				printText(1, "Press A to save");
				printText(2, "Press R2 to see previous value");
				printText(3, "Control the 4-bar on the left");
				printText(4, "joystick y axis");
				printVar(5, previousValue);
				printVar(6, currentValue);
				FourBar.move(GETJOY(LY));
				FourBar.move(GETJOY(LY));
				pros::delay(10);
				if(GETNEWPRESS(A)){
					std::ofstream fourBarDownWrite;
				  fourBarDownWrite.open("/usd/Maintanence/Four Bar Down.txt");
				  fourBarDownWrite << currentValue;
				  fourBarDownWrite.close();
					std::string line;
		  		fourBarDownRead.open("/usd/Maintanence/Four Bar Down.txt");
		  		if (fourBarDownRead.is_open()){
		    		while ( getline (fourBarDownRead,line) ){
		      		previousValue = std::stoi(line);
						}
		    	fourBarDownRead.close();
		  		}
				}
			}
		}
		else if(GETNEWPRESS(R1)){
			int previousValue = 0;
			std::string line;
  		std::ifstream fourBarCenterScoreRead ("/usd/Maintanence/Four Bar Center Score.txt");
  		if (fourBarCenterScoreRead.is_open()){
    		while ( getline (fourBarCenterScoreRead,line) ){
      		previousValue = std::stoi(line);
				}
    	fourBarCenterScoreRead.close();
  		}
			int currentValue = FourBarRotation.get_position();
			while(!GETNEWPRESS(L2)){//add a warning if not saved
				clearScreen();
				printConsole(FourBarRotation.get_angle());

				currentValue = FourBarRotation.get_position();
				printText(0, "4-BAR SCORE | PRESS L2 TO RETURN");
				printText(1, "Press A to save");
				printText(2, "Press R2 to see previous value");
				printText(3, "Control the 4-bar on the left");
				printText(4, "joystick y axis");
				printVar(5, previousValue);
				printVar(6, currentValue);
				FourBar.move(GETJOY(LY));
				FourBar.move(GETJOY(LY));
				pros::delay(10);
				if(GETNEWPRESS(A)){
					std::ofstream fourBarCenterScoreWrite;
				  fourBarCenterScoreWrite.open("/usd/Maintanence/Four Bar Center Score.txt");
				  fourBarCenterScoreWrite << currentValue;
				  fourBarCenterScoreWrite.close();
					std::string line;
		  		fourBarCenterScoreRead.open("/usd/Maintanence/Four Bar Center Score.txt");
		  		if (fourBarCenterScoreRead.is_open()){
		    		while ( getline (fourBarCenterScoreRead,line) ){
		      		previousValue = std::stoi(line);
						}
		    	fourBarCenterScoreRead.close();
		  		}
				}
			}
		}
		pros::delay(10);
	}
}

void twoBar(){
	while(!GETNEWPRESS(L2)){
		clearScreen();
		printText(0, "2-BAR    |    PRESS L2 TO RETURN");
		printText(1, "Press A to check motor temp");
		printText(2, "Press B to tune score height");
		printText(3, "Press X to tune down height");
		printText(4, "Press Y to check for stalling");
		if(GETNEWPRESS(A)){
			while(!GETNEWPRESS(L2)){
				printText(0, "2-BAR    |    PRESS L2 TO RETURN");
				// printVar(1, TwoBar.get_temperature());
				pros::delay(10);
			}
		}
		else if(GETNEWPRESS(B)){
			int previousValue = 0;
			std::string line;
  		std::ifstream twoBarScoreRead ("/usd/Maintanence/Two Bar Score.txt");
  		if (twoBarScoreRead.is_open()){
    		while ( getline (twoBarScoreRead,line) ){
      		previousValue = std::stoi(line);
				}
    	twoBarScoreRead.close();
  		}
			int currentValue = TwoBarRotation.get_position();
			while(!GETNEWPRESS(L2)){//add a warning if not saved
				clearScreen();
				currentValue = TwoBarRotation.get_position();
				printText(0, "2-BAR SCORE | PRESS L2 TO RETURN");
				printText(1, "Press A to save");
				printText(2, "Press R2 to go to previous value");
				printText(3, "Control the 2-bar on the left");
				printText(4, "joystick y axis");
				printVar(5, previousValue);
				printVar(6, currentValue);
				// TwoBar.move(GETJOY(LY));
				pros::delay(10);
				if(GETNEWPRESS(A)){
					std::ofstream twoBarScoreWrite;
				  twoBarScoreWrite.open("/usd/Maintanence/Two Bar Score.txt");
				  twoBarScoreWrite << currentValue;
				  twoBarScoreWrite.close();
					std::string line;
		  		twoBarScoreRead.open("/usd/Maintanence/Two Bar Score.txt");
		  		if (twoBarScoreRead.is_open()){
		    		while ( getline (twoBarScoreRead,line) ){
		      		previousValue = std::stoi(line);
						}
		    	twoBarScoreRead.close();
		  		}
				}
			}
		}
		else if(GETNEWPRESS(X)){
			int previousValue = 0;
			std::string line;
  		std::ifstream twoBarDownRead ("/usd/Maintanence/Two Bar Down.txt");
  		if (twoBarDownRead.is_open()){
    		while ( getline (twoBarDownRead,line) ){
      		previousValue = std::stoi(line);
				}
    	twoBarDownRead.close();
  		}
			int currentValue = TwoBarRotation.get_position();
			while(!GETNEWPRESS(L2)){//add a warning if not saved
				clearScreen();
				currentValue = TwoBarRotation.get_position();
				printText(0, "2-BAR SCORE | PRESS L2 TO RETURN");
				printText(1, "Press A to save");
				printText(2, "Press R2 to go to previous value");
				printText(3, "Control the 2-bar on the left");
				printText(4, "joystick y axis");
				printVar(5, previousValue);
				printVar(6, currentValue);
				// TwoBar.move(GETJOY(LY));
				pros::delay(10);
				if(GETNEWPRESS(A)){
					std::ofstream twoBarDownWrite;
				  twoBarDownWrite.open("/usd/Maintanence/Two Bar Down.txt");
				  twoBarDownWrite << currentValue;
				  twoBarDownWrite.close();
					std::string line;
		  		twoBarDownRead.open("/usd/Maintanence/Two Bar Down.txt");
		  		if (twoBarDownRead.is_open()){
		    		while ( getline (twoBarDownRead,line) ){
		      		previousValue = std::stoi(line);
						}
		    	twoBarDownRead.close();
		  		}
				}
			}
		}
		pros::delay(10);
	}
}

void roller(){
	while(!GETNEWPRESS(L2)){
		clearScreen();
		printText(0, "Roller   |   PRESS L2 TO RETURN");
		printText(1, "Press A to check motor temp");
		printText(2, "Press B to tune speed");
		printText(3, "Press Y to check for stalling");
		if(GETNEWPRESS(A)){
			while(!GETNEWPRESS(L2)){
				printText(0, "Roller   |   PRESS L2 TO RETURN");
				// printVar(1, TwoBar.get_temperature());
				pros::delay(10);
			}
		}
		else if(GETNEWPRESS(B)){
			int previousValue = 0;
			std::string line;
  		std::ifstream rollerSpeedRead ("/usd/Maintanence/Roller Speed.txt");
  		if (rollerSpeedRead.is_open()){
    		while ( getline (rollerSpeedRead,line) ){
      		previousValue = std::stoi(line);
				}
    	rollerSpeedRead.close();
  		}
			int currentValue = GETJOY(LY);
			while(!GETNEWPRESS(L2)){//add a warning if not saved
				clearScreen();
				currentValue = GETJOY(LY);
				printText(0, "ROLLER SPEED | PRESS L2 TO RETURN");
				printText(1, "Press A to save");
				printText(2, "Press R2 to see previous value");
				printText(3, "Control the 2-bar on the left");
				printText(4, "joystick y axis");
				printVar(5, previousValue);
				printVar(6, currentValue);
				if(GETBUTTON(R2)){
					Rollers.move(previousValue);
				}
				else{
					Rollers.move(GETJOY(LY));
				}
				pros::delay(10);
				if(GETNEWPRESS(A)){
					std::ofstream rollerSpeedWrite;
				  rollerSpeedWrite.open("/usd/Maintanence/Roller Speed.txt");
				  rollerSpeedWrite << abs(currentValue);
				  rollerSpeedWrite.close();
					std::string line;
		  		rollerSpeedRead.open("/usd/Maintanence/Roller Speed.txt");
		  		if (rollerSpeedRead.is_open()){
		    		while ( getline (rollerSpeedRead,line) ){
		      		previousValue = std::stoi(line);
						}
		    	rollerSpeedRead.close();
		  		}
				}
			}
		}
		pros::delay(10);
	}
}

void base(){
	while(!GETNEWPRESS(L2)){
		clearScreen();
		printText(0, "BASE    |    PRESS L2 TO RETURN");
		printText(1, "Press A to check motor temp");
		printText(2, "Press Y to check for stalling");
		if(GETNEWPRESS(A)){
			while(!GETNEWPRESS(L2)){
				clearScreen();
				printText(0, "BASE    |    PRESS L2 TO RETURN");
				printVar(1, LeftF.get_temperature());
				printVar(2, LeftB.get_temperature());
				printVar(3, RightF.get_temperature());
				printVar(4, RightB.get_temperature());
				pros::delay(10);
			}
		}
		if(GETNEWPRESS(Y)){
			while(!GETNEWPRESS(L2)){
				clearScreen();
				printText(0, "BASE    |    PRESS L2 TO RETURN");
				printText(1, "Press A to start the check");
				printText(2, "Once A is pressed, the robot will count down");
				printText(3, "count down from 3 second and spin the wheels");
				printText(4, "for 5 seconds");
				if(GETNEWPRESS(A)){
					clearScreen();
					printText(0, "BASE    |    PRESS L2 TO RETURN");
					printText(1, "Starting Motors in 3");
					master.rumble("-");
					printText(1, "Starting Motors in 2");
					master.rumble("-");
					printText(1, "Starting Motors in 1");
					master.rumble("-");
					int start = pros::millis();
					int now = pros::millis();
					clearScreen();
					printText(0, "Checking for base stalling...");
					bool leftFrontStalling = false;
					bool leftBackStalling = false;
					bool rightFrontStalling = false;
					bool rightBackStalling = false;
					while(now - start < 5000){
						LeftF.move(127);
						LeftB.move(-127);
						RightF.move(127);
						RightB.move(-127);
						if(now - start > 500 && (LeftF.get_power() > 1 || LeftF.get_actual_velocity() < 195)){
							leftFrontStalling = true;
						}
						if(now - start > 500 && (LeftB.get_power() > 1 || LeftB.get_actual_velocity() < 195)){
							leftBackStalling = true;
						}
						if(now - start > 500 && (RightF.get_power() > 1 || RightF.get_actual_velocity() < 195)){
							rightFrontStalling = true;
						}
						if(now - start > 500 && (RightB.get_power() > 1 || RightB.get_actual_velocity() < 195)){
							rightBackStalling = true;
						}
						pros::delay(10);
					}
					LeftF.move(0);
					LeftB.move(0);
					RightF.move(0);
					RightB.move(0);
					clearScreen();
					printText(0, "BASE    |    PRESS L2 TO RETURN");
					printText(1, "Results of stall check");
					printText(2, "1 means the motor is stalling");
					printText(3, "0 means the motor is NOT stalling");
					printVar(4, leftFrontStalling);
					printVar(5, leftBackStalling);
					printVar(6, rightFrontStalling);
					printVar(7, rightBackStalling);
					while(!GETNEWPRESS(L2)){
						pros::delay(10);
					}
				}
				pros::delay(10);
			}
		}
		pros::delay(10);
	}
}

void opcontrol() {
	pros::delay(100);
	FourBarRotation.reset();
	TwoBarRotation.reset();
	while (true){
		clearScreen();
		int brainBatteryLevel = pros::battery::get_capacity();
		int controllerBatteryLevel = master.get_battery_capacity();
		printText(0, "Hi Casey, Have fun maintaining!");
		printVar(1, brainBatteryLevel);
		printVar(2, controllerBatteryLevel);
		printText(3, "Press A for 4-bar options");
		printText(4, "Press B for 2-bar options");
		printText(5, "Press X for base options");
		printText(6, "Press Y for roller options");
		if(GETNEWPRESS(A)){
			fourBar();
		}
		else if(GETNEWPRESS(B)){
			twoBar();
		}
		else if(GETNEWPRESS(X)){
			base();
		}
		else if(GETNEWPRESS(Y)){
			roller();
		}
		pros::delay(10);
	}
}
