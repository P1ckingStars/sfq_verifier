#ifndef CTA_HPP
#define CTA_HPP

#include "automata.hpp"
#include <cstddef>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace ta {

typedef uint32_t automata_id;
typedef uint32_t channel_id;

struct pulse {
  automata_id id;
  letter_t letter;
  bool operator<(pulse const &p) const {
    return id < p.id || (id == p.id && letter < p.letter);
  }
};

struct PulseChannel {
  channel_id id;
  pulse in;
  set<pulse> out;
  void replace(vector<map<letter_t, letter_t>> &mp);
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
  vector<map<letter_t, PulseChannel *>> input_channels_;
  set<letter_t> letter_in_channel;
  map<letter_t, automata_id> letter4automata;
  map<letter_t, pulse> letter_map;
  PulseCAState *next(PulseCAState const *state, letter_t act,
                     set<letter_t> &output);

public:
  PulseCA(vector<Automata *> automata, vector<PulseChannel *> channels);
  Automata *to_dfa(); // TODOS
  void print_map() {
    cout << "Letter Map Table: " << endl;
    for (auto p : letter_map) {
      cout << "    " << p.first << ": " << p.second.id << "." << p.second.letter
           << endl;
    }
  }
  map<pulse, letter_t> pulse2letter_map() {
    map<pulse, letter_t> res;
    for (auto p : letter_map) {
      res[p.second] = p.first;
    }
    return res;
  }
};

class CARefinedAutomata : Automata {
  vector<vector<letter_t>> act_mp_;

public:
  CARefinedAutomata(Automata *automata, vector<vector<letter_t>> act_mp_);
  letter_t act2letter(PulseCAAct act) { return act_mp_[act.machine][act.act]; }
};

} // namespace ta

#endif
