#include "robot.hpp"

void Piston::setState(PistonState istate){
  state = istate;
  bool newState = static_cast<bool>(state);
  piston->set_value(reverse ? !newState : newState);
}

void Piston::switchState(){
  if(state == PistonState::extend){
    state = PistonState::retract;
    piston->set_value(static_cast<bool>(PistonState::retract));
    printBrainText(2, "retract");
  }
  else{
    state = PistonState::extend;
    piston->set_value(static_cast<bool>(PistonState::extend));
    printBrainText(2, "extend");
  }
}

void Piston::extend(){
  state = PistonState::extend;
  bool newState = static_cast<bool>(state);
  piston->set_value(reverse ? !newState : newState);
}

void Piston::retract(){
  state = PistonState::retract;
  bool newState = static_cast<bool>(state);
  piston->set_value(reverse ? !newState : newState);
}
