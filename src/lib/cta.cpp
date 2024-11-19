
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
  for (int i = 0; i < channels_.size(); i++) {
    PulseChannel *c = channels_[i];
    c->from_act = input_map[c->from][c->from_act];
    c->to_act = input_map[c->to][c->to_act];
    input_channels_[c->from][c->from_act] = channels_[i];
    output_channels_[c->to][c->to_act] = channels_[i];
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
  init_state->channel_states = vector<bool>(this->channels_.size(), 0);
  unordered_map<string, state_t> ca2dfa;
  deque<PulseCAState *> q;
  APPEND_STATE(init_state);
  while (!q.empty()) {
    auto state = q.front();
    q.pop_front();
    for (letter_t i = 0; i < this->total_letter; i++) {
      PulseCAState *next_state;
      string next_state_query;
      if ((next_state = this->next(state, i)) &&
          !ca2dfa.count(next_state_query = next_state->to_str())) {
        APPEND_STATE(next_state);
        Edge e;
        e.from = ca2dfa[state->to_str()];
        e.to = ca2dfa[next_state_query];
        e.letters.insert(i);
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
  state_t n_state;
  if ((n_state = automatas_[a_id]->next(state->states[a_id], act)) !=
      STATE_NOT_EXISTS) {
    if (output_channels_[a_id].count(act)) {
      PulseChannel const *ch = output_channels_[a_id][act];
      if (state->channel_states[ch->id]) {
        PulseCAState *new_state = new PulseCAState();
        *new_state = *state;
        new_state->states[a_id] = n_state;
        new_state->channel_states[ch->id] = false;
        return new_state;
      } else
        return nullptr;
    }
    if (input_channels_[a_id].count(act)) {
      PulseChannel const *ch = input_channels_[a_id][act];
      if (!state->channel_states[ch->id]) {
        PulseCAState *new_state = new PulseCAState();
        *new_state = *state;
        new_state->states[a_id] = n_state;
        new_state->channel_states[ch->id] = true;
        return new_state;
      } else
        return nullptr;
    }
    PulseCAState *new_state = new PulseCAState();
    *new_state = *state;
    new_state->states[a_id] = n_state;
    return new_state;
  }
  return nullptr;
}

} // namespace ta
