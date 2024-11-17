#ifndef CTA_HPP
#define CTA_HPP

#include "automata.hpp"
#include "timed_automata.hpp"
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <iostream>


namespace ta {

typedef uint32_t automata_id;

class PulseChannel {
    automata_id from;
    automata_id to;
    letter_t from_act;
    letter_t to_act;
};

class PulseCAState {
    vector<state_t> states;
    vector<bool> channel_states;
public:
    bool operator==(PulseCAState const & s) {
        return s.states == this->states && channel_states == this->channel_states;
    }
    string to_str();
};

struct PulseCAAct {
    automata_id machine;
    letter_t act;
};

class PulseCA {
    vector<Automata *> automatas_;
    vector<PulseChannel *> channels_;
public:
    PulseCA(vector<Automata *> automata, vector<PulseChannel *> channels);
    Automata * to_dfa(); // TODOS
};

class CARefinedAutomata: Automata {
    vector<vector<letter_t>> act_mp_; 
public:
    CARefinedAutomata(Automata * automata, vector<vector<letter_t>> act_mp_);
    letter_t act2letter(PulseCAAct act) {
        return act_mp_[act.machine][act.act];
    }
};

}

#endif
