#include "region_timed_automata.hpp"
#include "timed_automata.hpp"
#include <unordered_set>

using namespace ta;

string ClockState::to_string() {
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

bool TimedState::operator==(TimedState const &state) {
  bool res;
  if (clock_state->orders.size() != state.clock_state->orders.size() ||
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
    if (clock_state->clock_ticks_[i] != state.clock_state->clock_ticks_[i])
      return false;
  }
  return true;
}

string TimedState::to_string() {
  stringstream ss;
  ss << "state: " << state_ << " \n";
  ss << clock_state->to_string();
  return ss.str();
}

void ClockState::advance() {
  if (this->equal[0]) {
    this->equal[0] = false;
    return;
  }
  clock_id last_clk = orders[orders.size() - 1];
  for (int i = this->orders.size() - 1; i > 0; i--) {
    this->orders[i] = this->orders[i - 1];
    this->equal[i] = this->equal[i - 1];
  }
  clock_ticks_[last_clk]++;
  this->orders[0] = last_clk;
  this->equal[0] = true;
}

void ClockState::set_zero(unordered_set<clock_id> const &clock) {
  int found = false;
  for (int i = this->orders.size() - 1; i > 0; i--) {
    if (clock.count(this->orders[i]))
      found++;
    if (found) {
      this->orders[i] = this->orders[i - found];
      this->equal[i] = this->equal[i - found];
    }
  }
  int i = 0;
  for (auto clk : clock) {
    this->orders[i] = clk;
    this->equal[i] = true;
    this->clock_ticks_[clk] = 0;
    i++;
  }
}

RegionTransitionTable::RegionTransitionTable(TransitionTable *transition_table) {
    
}






