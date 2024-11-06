#ifndef REGION_TIMED_AUTOMATA
#define REGION_TIMED_AUTOMATA

#include "timed_automata.hpp"
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct ClockFraction {
  vector<clock_id> orders;
  vector<bool> equal;
  vector<int> clock_ticks_;
  string to_string() {
    stringstream ss;
    for (int i = 0; i < orders.size(); i++) {
      ss << orders[i] << equal[i];
    }
    ss << "\n";
    for (int i = 0; i < clock_ticks_.size(); i++) {
      ss << clock_ticks_[i] << " ";
    }
    return ss.str();
  }

};

class TimedState {
  int state_;
  ClockFraction * clock_state;

public:
  bool operator==(TimedState const &state) {
    bool res;
    if (clock_state->orders.size() !=
            state.clock_state->orders.size() ||
        clock_state->equal.size() != state.clock_state->equal.size() ||
        state_ != state.state_ ||
        clock_state->clock_ticks_ != state.clock_state->clock_ticks_) {
      return false;
    }
    for (int i = 0; i < clock_state->orders.size(); i++) {
      if (clock_state->orders[i] != state.clock_state->orders[i])
        return false;
    }
    for (int i = 0; i < clock_state->equal.size(); i++) {
      if (clock_state->equal[i] != state.clock_state->equal[i])
        return false;
    }
    for (int i = 0; i < clock_state->orders.size(); i++) {
      if (clock_state->clock_ticks_[i] !=
          state.clock_state->clock_ticks_[i])
        return false;
    }
    return true;
  }
  string to_string() {
    stringstream ss;
    ss << "state: " << state_ << " \n";
    ss << clock_state->to_string();
    return ss.str();
  }
};

class RegionTransitionTable {
  int num_inputs;
  int num_clock;
  vector<int> state_;
};

#endif
