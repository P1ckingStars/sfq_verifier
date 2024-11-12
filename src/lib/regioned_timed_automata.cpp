#include "region_timed_automata.hpp"
#include "timed_automata.hpp"
#include <deque>
#include <unordered_set>

using namespace ta;

string ClockState::to_string() const {
  stringstream ss;
  for (int i = 0; i < orders.size(); i++) {
    ss << orders[i] << equal[i];
  }
  ss << "\n";
  for (int i = 0; i < clock_ticks.size(); i++) {
    ss << clock_ticks[i] << " ";
  }
  return ss.str();
}

bool TimedState::operator==(TimedState const &state) {
  bool res;
  if (this->clock_state->orders.size() != state.clock_state->orders.size() ||
      this->clock_state->equal.size() != state.clock_state->equal.size() ||
      this->state != state.state ||
      this->clock_state->clock_ticks != state.clock_state->clock_ticks) {
    return false;
  }
  for (int i = 0; i < clock_state->orders.size(); i++) {
    if (clock_state->orders[i] != state.clock_state->orders[i])
      return false;
    if (clock_state->equal[i] != state.clock_state->equal[i])
      return false;
  }
  for (int i = 0; i < clock_state->orders.size(); i++) {
    if (this->clock_state->clock_ticks[i] != state.clock_state->clock_ticks[i])
      return false;
  }
  return true;
}

string TimedState::to_string() {
  stringstream ss;
  ss << "state: " << state << " \n";
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
  clock_ticks[last_clk]++;
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
    this->clock_ticks[clk] = 0;
    i++;
  }
}

inline ClockState *
clock_state_query(ClockState *clk,
                  unordered_map<string, ClockState *> &str2clk) {
  string q = clk->to_string();
  if (str2clk.count(q)) {
    delete clk;
    str2clk[q];
  } else {
    return str2clk[q] = clk;
  }
}

inline TimedState const *
timed_state_query(TimedState *ts,
                  unordered_map<string, timed_state_id> &str2sid,
                  vector<TimedState const *> &states) {
  string q = ts->to_string();
  if (str2sid.count(q)) {
    delete ts;
    return states[str2sid[q]];
  } else {
    str2sid[q] = states.size();
    states.push_back(ts);
    return ts;
  }
}
RegionTransitionTable::RegionTransitionTable(
    TransitionTable *transition_table) {
  this->num_clock = transition_table->num_clocks;
  unordered_map<string, ClockState *> str2clk;
  TimedState *initial_state = new TimedState();
  ClockState *zero_clk_state = new ClockState();

#define CLOCK_STATE(clk) clock_state_query(clk, str2clk)
#define TIMED_STATE(s) timed_state_query(s, state2id_, states_)

  initial_state->clock_state = CLOCK_STATE(zero_clk_state);
  initial_state->state = 0;
  deque<TimedState const *> q;
  q.push_back(TIMED_STATE(initial_state));

  while (!q.empty()) {
    auto curr_state = q.front();
    auto curr_clk = curr_state->clock_state;
    q.pop_front();
    state_id untimed_state = curr_state->state;

  }
}













