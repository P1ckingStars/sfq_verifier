/*
class AND(SFQ):
    ''' Sync. AND element

    Default numbers for JJs, setup time, and firing delay come from [1] (Table III and prose);
    I've set an arbitrary number a little larger than setup time for the hold time.
    Setup and hold time come from [2] (Figure 29.3.1) (it also uses: 7.9 for
    firing delay and has 14 JJs)
    '''
    _setup_time = 2.8  # Time before clock during which no inputs are allowed
    _hold_time = 3.0  # Time after clock during which no inputs are allowed

    name = 'AND'
    inputs = ['a', 'b', 'clk']
    outputs = ['q']
    transitions = [
        {'id': '0',  'source': 'idle',            'trigger': 'clk',      'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time}},
        {'id': '1',  'source': 'idle',            'trigger': 'a',        'dest': 'a_arrived'},
        {'id': '2',  'source': 'idle',            'trigger': 'b',        'dest': 'b_arrived'},
        {'id': '3',  'source': 'a_arrived',       'trigger': 'b',        'dest': 'a_and_b_arrived'},
        {'id': '4',  'source': 'a_arrived',       'trigger': 'a',        'dest': 'a_arrived'},
        {'id': '5',  'source': 'a_arrived',       'trigger': 'clk',      'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time}},
        {'id': '6',  'source': 'b_arrived',       'trigger': 'a',        'dest': 'a_and_b_arrived'},
        {'id': '7',  'source': 'b_arrived',       'trigger': 'clk',      'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time}},
        {'id': '8',  'source': 'b_arrived',       'trigger': 'b',        'dest': 'b_arrived'},
        {'id': '9',  'source': 'a_and_b_arrived', 'trigger': 'clk',      'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time},
         'firing': 'q'},
        {'id': '10', 'source': 'a_and_b_arrived', 'trigger': ['a', 'b'], 'dest': 'a_and_b_arrived'},
    ]
    jjs = 11
    firing_delay = 9.2

*/

#include "automata.hpp"
#include "gates.hpp"

#define CLK 0
#define A   1
#define B   2
#define C   3

#define IDLE 0
#define A_ARRIVED 1
#define B_ARRIVED 2
#define A_AND_B_ARRIVED 3

using namespace ta;

Automata * AND_GATE() {
    Automata * res = new Automata();
    res->appendNode(); // s_0: idle
    res->appendNode(); // s_1: a_arrived
    res->appendNode(); // s_2: b_arrived
    res->appendNode(); // s_3: a_and_b_arrived
    res->appendNode(); // s_4: fire
    
    res->appendEdge(Edge(IDLE, IDLE, CLK, NO_OUTPUT));
    res->appendEdge(Edge(IDLE, A_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(IDLE, B_ARRIVED, B, NO_OUTPUT));
    res->appendEdge(Edge(A_ARRIVED, A_AND_B_ARRIVED, B, NO_OUTPUT));
    res->appendEdge(Edge(A_ARRIVED, A_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(A_ARRIVED, IDLE, CLK, NO_OUTPUT));
    res->appendEdge(Edge(B_ARRIVED, A_AND_B_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(B_ARRIVED, IDLE, CLK, NO_OUTPUT));
    res->appendEdge(Edge(B_ARRIVED, B_ARRIVED, B, NO_OUTPUT));
    res->appendEdge(Edge(A_AND_B_ARRIVED, IDLE, CLK, C));
    res->appendEdge(Edge(A_AND_B_ARRIVED, A_AND_B_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(A_AND_B_ARRIVED, A_AND_B_ARRIVED, B, NO_OUTPUT));
    res->full_reduce();

    return res;
}
