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
  vector<clock_id> orders;
  vector<bool> equal;
  vector<int> clock_ticks_;
  string to_string();
  void advance();
  void set_zero(unordered_set<clock_id> const &clock);
};

class TimedState {
  int state_;
  ClockState *clock_state;

public:
  bool operator==(TimedState const &state);
  string to_string();
};

typedef int timed_state_id;
class RegionTransitionTable {
  int num_inputs;
  int num_clock;
  vector<vector<timed_state_id>> graph_;
  vector<TimedState const *> state_;
  unordered_map<string, timed_state_id> state2id; 
  RegionTransitionTable(TransitionTable *transition_table);
};

}
#endif
