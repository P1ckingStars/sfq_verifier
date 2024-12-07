
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
void PulseChannel::replace(vector<map<letter_t, letter_t>> &mp) {
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
  vector<map<letter_t, letter_t>> input_map(automatas_.size());
  for (int i = 0; i < automata.size(); i++) {
    int prev_idx = idx;
    idx = automatas_[i]->replace_input(idx, input_map[i]);
    for (int k = prev_idx; k < idx; k++) {
      letter4automata[k] = i;
    }
  }
  for (automata_id i = 0; i < input_map.size(); i++) {
    for (auto p : input_map[i]) {
      this->letter_map[p.second] = {i, p.first};
    }
  }
  this->input_channels_ =
      vector<map<letter_t, PulseChannel *>>(automatas_.size());
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
  map<set<letter_t>, letter_t> mp;
  while (!q.empty()) {
    auto state = q.front();
    q.pop_front();
    cout << "CURRENT STATE: " << state->to_str() << endl;
    for (letter_t i = 0; i < this->total_letter; i++) {
      if (letter_in_channel.count(i))
        continue;
      cout << "state: " << ca2dfa[state->to_str()] << " try act " << i << endl;
      PulseCAState *next_state;
      set<letter_t> output;
      string next_state_query;
      if (!!(next_state = this->next(state, i, output))) {
        if (!ca2dfa.count(next_state_query = next_state->to_str())) {
          cout << "NEXT STATE: " << next_state->to_str() << endl;
          APPEND_STATE(next_state);
        }
        letter_t edge_out = NO_OUTPUT;
        if (!output.empty()) {
          if (!mp.count(output)) {
            mp.insert({output, mp.size()});
          }
          edge_out = mp[output] + letter_map.size();
        }
        Edge e(ca2dfa[state->to_str()], ca2dfa[next_state_query], i, edge_out);
        res->appendEdge(e);
      }
    }
  }
  return res;
}

PulseCAState *PulseCA::next(PulseCAState const *state, letter_t act,
                            set<letter_t> &output) {
  automata_id a_id = this->letter4automata[act];
  pair<state_t, letter_t> n_state;
  deque<pulse> pulses;
  vector<bool> visited(this->automatas_.size());
#define GO_VISIT(p)                                                            \
  {                                                                            \
    visited[p.id] = true;                                                      \
    pulses.push_back(p);                                                       \
  }
  pulse init_pulse = {a_id, act};
  GO_VISIT(init_pulse);
  PulseCAState *new_state = new PulseCAState();
  *new_state = *state;
  while (!pulses.empty()) {
    pulse p = pulses.front();
    pulses.pop_front();
    if ((n_state = automatas_[p.id]->next(new_state->states[p.id], p.letter))
            .first != STATE_NOT_EXISTS) {
      new_state->states[p.id] = n_state.first;
      if (n_state.second != NO_OUTPUT) {
        if (this->input_channels_[p.id].count(n_state.second)) {
          for (auto new_p : this->input_channels_[p.id][n_state.second]->out) {
            /*
            if (visited[new_p.id]) {
              delete new_state;
              cout << "ERROR: loop" << endl;
              return nullptr;
            }
            */
            cout << "signal propagate " << new_p.id << " " << new_p.letter
                 << endl;
            pulses.push_back(new_p);
          }
        } else {
          output.insert(n_state.second);
        }
      }
    } else {
      delete new_state;
      cout << "ERROR: state not exists" << endl;
      return nullptr;
    }
  }
  cout << "SUCCEED" << endl;
  return new_state;
} // namespace ta

} // namespace ta
