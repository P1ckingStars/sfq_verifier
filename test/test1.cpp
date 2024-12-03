

#include "gates.hpp"
#include "automata.hpp"
#include "cta.hpp"
#include <iostream>
#include <vector>

using namespace ta;
using namespace std;

void and_test1(Automata * automata) {

    letter_t arr[] = {0, 1, 2, 0, 3, 1, 0, 3, 1, 2, 3, 0, 3, 2, 1, 0, 3};
    vector<letter_t> in_seq = {arr, arr + (sizeof(arr)/sizeof(letter_t))};
    vector<letter_t> out_seq;
    automata->run(in_seq, out_seq);
    for (auto i : out_seq)
        cout << i << " ";
    cout << endl;
}

int main() {

    vector<Automata *> automatas;
    vector<PulseChannel *> channels;
    Automata * and_gate = AND_GATE();
    Automata * clk  = CLK_UNIT();
    Automata * wire = WIRE();

    cout << "AND transition table: " << endl;
    and_gate->print_transition_table();
    cout << "CLK transition table: " << endl;
    clk->print_transition_table();
    and_test1(and_gate);   

    automatas.push_back(and_gate);
    automatas.push_back(clk);
    automatas.push_back(wire);
    PulseChannel * clk_channel = new PulseChannel();
    clk_channel->in.id = 1;
    clk_channel->in.letter = 0;
    clk_channel->out.insert({0, 0});
    PulseChannel * fire_channel = new PulseChannel();
    fire_channel->in.id = 1;
    fire_channel->in.letter = 1;
    fire_channel->out.insert({2, 0});
    PulseChannel * output_channel = new PulseChannel();
    output_channel->in.id = 0;
    output_channel->in.letter = 3;
    output_channel->out.insert({2, 1});
    channels.push_back(clk_channel);
    channels.push_back(fire_channel);
    channels.push_back(output_channel);
    PulseCA * pca = new PulseCA(automatas, channels);
    auto dfa = pca->to_dfa();
    pca->print_map();
    cout << "AND transition table: " << endl;
    and_gate->print_transition_table();
    cout << "CLK transition table: " << endl;
    clk->print_transition_table();
    cout << "and cell state: " << and_gate->num_states() << " and circuit: " << dfa->num_states() << endl;
    dfa->print_transition_table();
    dfa->mergeEdge();
    cout << "and cell state: " << and_gate->num_states() << " and circuit: " << dfa->num_states() << endl;
    dfa->print_transition_table();
    dfa->full_reduce();
    cout << "and cell state: " << and_gate->num_states() << " and circuit: " << dfa->num_states() << endl;
    dfa->print_transition_table();
}
