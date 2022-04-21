#include "main.h"

class Button{
private:
  int port;
  bool prev;

public:
  Button(){}
  Button(int iport){
    port = iport;
    prev = false;
  }

  int getPort(){
    return port;
  }


  void setPort(int iport){
    port = iport;
  }

  bool getPress();
  bool getNewPress();
};
