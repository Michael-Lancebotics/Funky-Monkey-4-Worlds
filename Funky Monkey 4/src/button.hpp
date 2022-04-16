#include "main.h"

class Button{
private:
  int port;

public:
  Button(){}
  Button(int iport){
    port = iport;
  }

  int getPort(){
    return port;
  }

  bool getPressed(){
    return pros::ADIDigitalIn(port).get_value();
  }

  void setPort(int iport){
    port = iport;
  }
};
