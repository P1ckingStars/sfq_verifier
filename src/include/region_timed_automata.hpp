#ifndef REGION_TIMED_AUTOMATA
#define REGION_TIMED_AUTOMATA

#include "timed_automata.hpp"
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

namespace ta {


struct ClockState {
  // orders and equals can give you a sequence like this:
  // 0 = c_2 < c_1 < c_4 = c_3
  ClockMax const * limits;
  vector<clock_id> orders;
  vector<bool> equal;
  vector<int> clock_ticks;
  string to_string() const;
  void advance();
  void set_zero(unordered_set<clock_id> const &clock);
};

struct TimedState {
  int state;
  ClockState const *clock_state;
  bool operator==(TimedState const &state);
  string to_string();
};

typedef int timed_state_id;
class RegionTransitionTable {
  int num_inputs;
  int num_clock;
  vector<unordered_map<input_id, timed_state_id>> graph_;
  vector<TimedState const *> states_;
  unordered_map<string, timed_state_id> state2id_;
  RegionTransitionTable(TransitionTable *transition_table);
};

} // namespace ta
#endif
