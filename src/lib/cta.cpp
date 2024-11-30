
#include "cta.hpp"
#include "automata.hpp"
#include <deque>
#include <sstream>
#include <unordered_map>

namespace ta {
string PulseCAState::to_str() {
  stringstream ss;
  ss << "states: ";
  for (int i = 0; i < this->states.size(); i++) {
    ss << this->states[i] << " ";
  }
  ss << "\nchannel states: ";

  return ss.str();
}
void PulseChannel::replace(vector<unordered_map<letter_t, letter_t>> &mp) {
  this->in.letter = mp[this->in.id][this->in.letter];
  set<pulse> new_out;
  for (auto p : this->out) {
    new_out.insert({p.id, mp[p.id][p.letter]});
  }
  this->out = new_out;
}

PulseCA::PulseCA(vector<Automata *> automata, vector<PulseChannel *> channels)
    : automatas_(automata), channels_(channels) {
  int idx = 0;
  vector<unordered_map<letter_t, letter_t>> input_map(automatas_.size());
  for (int i = 0; i < automata.size(); i++) {
    int prev_idx = idx;
    idx = automatas_[i]->replace_input(idx, input_map[i]);
    for (int k = prev_idx; k < idx; k++) {
      letter4automata[k] = i;
    }
  }
  this->input_channels_ =
      vector<unordered_map<letter_t, PulseChannel *>>(automatas_.size());
  for (int i = 0; i < channels_.size(); i++) {
    PulseChannel *c = channels_[i];
    c->replace(input_map);
    this->letter_in_channel.insert(c->in.letter);
    for (auto p : c->out) {
      this->letter_in_channel.insert(p.letter);
    }
    this->input_channels_[c->in.id][c->in.letter] = c;
  }
  this->total_letter = idx;
}

#define APPEND_STATE(s)                                                        \
  {                                                                            \
    res->appendNode();                                                         \
    ca2dfa[s->to_str()] = ca2dfa.size();                                       \
    q.push_back(s);                                                            \
  }
Automata *PulseCA::to_dfa() {
  Automata *res = new Automata();
  PulseCAState *init_state = new PulseCAState();
  init_state->states = vector<state_t>(this->automatas_.size(), 0);
  unordered_map<string, state_t> ca2dfa;
  deque<PulseCAState *> q;
  APPEND_STATE(init_state);
  while (!q.empty()) {
    auto state = q.front();
    q.pop_front();
    for (letter_t i = 0; i < this->total_letter; i++) {
      if (letter_in_channel.count(i))
        continue;
      PulseCAState *next_state;
      string next_state_query;
      if ((next_state = this->next(state, i)) &&
          !ca2dfa.count(next_state_query = next_state->to_str())) {
        APPEND_STATE(next_state);
        Edge e(ca2dfa[state->to_str()], ca2dfa[next_state_query], i, NO_OUTPUT);
        res->appendEdge(e);
      }
    }
  }
  res->mergeEdge();
  res->full_reduce();
  return res;
}

PulseCAState *PulseCA::next(PulseCAState const *state, letter_t act) {
  automata_id a_id = this->letter4automata[act];
  pair<state_t, letter_t> n_state;
  deque<pulse> pulses;
  vector<bool> visited(this->automatas_.size());
#define GO_VISIT(p)                                                            \
  {                                                                            \
    visited[a_id] = true;                                                      \
    pulses.push_back(p);                                                       \
  }
  pulse init_pulse = {a_id, act};
  GO_VISIT(init_pulse);
  PulseCAState *new_state = new PulseCAState();
  *new_state = *state;
  while (!pulses.empty()) {
    pulse p = pulses.front();
    pulses.pop_front();
    if ((n_state = automatas_[p.id]->next(state->states[a_id], act)).first !=
        STATE_NOT_EXISTS) {
      new_state->states[a_id] = n_state.first;
      if (n_state.second != NO_OUTPUT &&
          this->input_channels_[a_id].count(n_state.second)) {
        for (auto new_p : this->input_channels_[a_id][n_state.second]->out) {
          if (visited[new_p.id]) {
            delete new_state;
            return nullptr;
          }
          pulses.push_back(new_p);
        }
      }
    } else {
      delete new_state;
      return nullptr;
    }
  }
  return new_state;
} // namespace ta

} // namespace ta
