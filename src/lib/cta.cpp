
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
  for (int i = 0; i < this->channel_states.size(); i++) {
    ss << this->channel_states[i] << " ";
  }
  return ss.str();
}

PulseCA::PulseCA(vector<Automata *> automata, vector<PulseChannel *> channels) : automatas_(automata), channels_(channels) {
    int idx = 0;
    vector<unordered_map<letter_t, letter_t>> input_map(automatas_.size());
    for (int i = 0; i < automata.size(); i++) {
        idx = automatas_[i]->replace_input(idx, input_map[i]);
    }

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
  init_state->channel_states = vector<bool>(this->channels_.size(), 0);
  unordered_map<string, state_t> ca2dfa;
  deque<PulseCAState *> q;
  APPEND_STATE(init_state);
  while (!q.empty()) {
    auto state = q.front();
    q.pop_front();
  }

  return res;
}

} // namespace ta
