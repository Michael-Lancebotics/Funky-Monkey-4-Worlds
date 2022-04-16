#include "main.h"

class Rotation{
private:
  unsigned int port;
  bool reverse;

public:
  Rotation(){}
  Rotation(int iport, bool ireverse){
    port = iport;
    reverse = ireverse;
  }

  int getPort(){
    return port;
  }

  bool getReverse(){
    return reverse;
  }
  
  void setPort(int iport){
    port = iport;
  }

  void setReverse(int ireverse){
    reverse = ireverse;
  }

  int getValue();
  void setPosition(int pos);
  void reset();

};
