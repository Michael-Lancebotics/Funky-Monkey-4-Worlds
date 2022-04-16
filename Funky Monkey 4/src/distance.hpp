#include "main.h"

class Distance{
private:
  int port;
  int distance;

public:
  Distance(){}
  Distance(int iport){
    port = iport;
  }

  int getPort(){
    return port;
  }

  int getFilteredDistance(){
    return distance;
  }

  int getDistance(){
    return pros::Distance(port).get();
  }

  void setFilteredDistance(int idistance){
    distance = idistance;
  }

  void setPort(int iport){
    port = iport;
  }
};
