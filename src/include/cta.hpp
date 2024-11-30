#ifndef CTA_HPP
#define CTA_HPP

#include "automata.hpp"
#include "timed_automata.hpp"
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <variant>
#include <vector>

namespace ta {

typedef uint32_t automata_id;
typedef uint32_t channel_id;

struct pulse {
  automata_id id;
  letter_t letter;
  bool operator<(pulse const &p) {
    return id < p.id || (id == p.id && letter < p.letter);
  }
};
struct PulseChannel {
  channel_id id;
  pulse in;
  set<pulse> out;
  void replace(vector<unordered_map<letter_t, letter_t>> &mp);
};

struct PulseCAState {
  vector<state_t> states;
  bool operator==(PulseCAState const &s) { return s.states == this->states; }
  string to_str();
};

struct PulseCAAct {
  automata_id machine;
  letter_t act;
};

class PulseCA {
  size_t total_letter;
  vector<Automata *> automatas_; // Automata may share the same input characters
                                 // when they first initialized, but shouldn't
                                 // use the same inputs here, must differentiate
  vector<PulseChannel *> channels_;
  vector<unordered_map<letter_t, PulseChannel *>> input_channels_;
  set<letter_t> letter_in_channel;
  unordered_map<letter_t, automata_id> letter4automata;
  PulseCAState *next(PulseCAState const *state, letter_t act);

public:
  PulseCA(vector<Automata *> automata, vector<PulseChannel *> channels);
  Automata *to_dfa(); // TODOS
};

class CARefinedAutomata : Automata {
  vector<vector<letter_t>> act_mp_;

public:
  CARefinedAutomata(Automata *automata, vector<vector<letter_t>> act_mp_);
  letter_t act2letter(PulseCAAct act) { return act_mp_[act.machine][act.act]; }
};

} // namespace ta

#endif
