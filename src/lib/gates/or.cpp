/*
class OR(SFQ):
    ''' Sync. OR element

    Default numbers for JJs and firing delay come from [1].
    Setup/hold time comes from [2].
    '''
    _setup_time = 5.8
    _hold_time = 0.0

    name = 'OR'
    inputs = ['a', 'b', 'clk']
    outputs = ['q']
    transitions = [
        {'id': '0', 'source': 'idle',           'trigger': ['a', 'b'], 'dest': 'a_or_b_arrived'},
        {'id': '1', 'source': 'idle',           'trigger': 'clk',      'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time}},
        {'id': '2', 'source': 'a_or_b_arrived', 'trigger': ['a', 'b'], 'dest': 'a_or_b_arrived'},
        {'id': '3', 'source': 'a_or_b_arrived', 'trigger': 'clk',      'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time},
         'firing': 'q'},
    ]
    jjs = 12
    firing_delay = 8.0
*/

#include "automata.hpp"
#include "gates.hpp"

#define CLK 0
#define A   1
#define B   2
#define C   3

#define IDLE 0
#define A_OR_B_ARRIVED 1 

using namespace ta;

Automata * OR_GATE () {
    Automata * res = new Automata();
    res -> appendNode(); //IDLE
    res -> appendNode(); //A_OR_B_ARRIVED
    
    res -> appendEdge(Edge(IDLE, IDLE, CLK, NO_OUTPUT));
    res -> appendEdge(Edge(IDLE, A_OR_B_ARRIVED, A, NO_OUTPUT));
    res -> appendEdge(Edge(IDLE, A_OR_B_ARRIVED, B, NO_OUTPUT));
    res -> appendEdge(Edge(A_OR_B_ARRIVED, A_OR_B_ARRIVED, A, NO_OUTPUT));
    res -> appendEdge(Edge(A_OR_B_ARRIVED, A_OR_B_ARRIVED, B, NO_OUTPUT));
    res -> appendEdge(Edge(A_OR_B_ARRIVED, IDLE, CLK, C));
    res -> full_reduce();
    
    return res;
}
