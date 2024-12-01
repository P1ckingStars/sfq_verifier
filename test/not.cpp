/*
class INV(SFQ):
    ''' Sync. inverter element

    Numbers for come from Nagaoka 2019 [2].
    '''
    _setup_time = 1.2
    _hold_time = 5.0

    name = 'INV'
    inputs = ['a', 'clk']
    outputs = ['q']
    transitions = [
        {'id': '0', 'source': 'idle',      'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time,    'past_constraints': {'*': _setup_time},
         'firing': 'q'},
        {'id': '1', 'source': 'idle',      'trigger': 'a',   'dest': 'a_arrived'},
        {'id': '2', 'source': 'a_arrived', 'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time,    'past_constraints': {'*': _setup_time}},
        {'id': '3', 'source': 'a_arrived', 'trigger': 'a',   'dest': 'a_arrived'},
    ]
    jjs = 10
    firing_delay = 9.6

*/


#include "automata.hpp"

#define CLK 0
#define A   1
#define FIRE 2
#define C   3


#define IDLE 0
#define A_ARRIVED 1
#define OUTPUT 2

using namespace ta;

Automata * NOT_GATE() {
    Automata * res = new Automata();
    res->appendNode(); // s_0
    res->appendNode(); // s_1: a_arrive
    res->appendEdge(Edge(IDLE, A_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(IDLE, OUTPUT, CLK, NO_OUTPUT));
    res->appendEdge(Edge(OUTPUT, IDLE, FIRE, C));
    res->appendEdge(Edge(A_ARRIVED, A_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(A_ARRIVED, IDLE, CLK, NO_OUTPUT));
    res.full_reduce();
    return res;
}