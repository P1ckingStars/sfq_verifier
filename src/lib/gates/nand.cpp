/*

class NAND(SFQ):
    ''' Sync. NAND element

    Default JJ numbers come from [4].
    '''
    _setup_time = 5.0
    _hold_time = 0.0

    name = 'NAND'
    inputs = ['a', 'b', 'clk']
    outputs = ['q']
    transitions = [
        # Order if simultaneous is based on order of transitions given for a given source,
        # meaning here, while in 'idle', 'a' will be handled first, then 'b', then finally 'clk',
        # if they arrive simultaneously.
        {'id': '0',  'source': 'idle',      'trigger': 'a',   'dest': 'a_arrived'},
        {'id': '1',  'source': 'idle',      'trigger': 'b',   'dest': 'b_arrived'},
        {'id': '2',  'source': 'idle',      'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time},
         'firing': 'q'},

        {'id': '3',  'source': 'a_arrived', 'trigger': 'a',   'dest': 'a_arrived'},
        {'id': '4',  'source': 'a_arrived', 'trigger': 'b',   'dest': 'a_and_b_arrived'},
        {'id': '5',  'source': 'a_arrived', 'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time},
         'firing': 'q'},

        {'id': '6',  'source': 'b_arrived', 'trigger': 'b',   'dest': 'b_arrived'},
        {'id': '7',  'source': 'b_arrived', 'trigger': 'a',   'dest': 'a_and_b_arrived'},
        {'id': '8',  'source': 'b_arrived', 'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time},
         'firing': 'q'},

        {'id': '9',  'source': 'a_and_b_arrived', 'trigger': 'b',   'dest': 'a_and_b_arrived'},
        {'id': '10', 'source': 'a_and_b_arrived', 'trigger': 'a',   'dest': 'a_and_b_arrived'},
        {'id': '11', 'source': 'a_and_b_arrived', 'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time}},
    ]
    jjs = 16
    firing_delay = 5.0


*/


#include "automata.hpp"
#include "gates.hpp"

#define CLK 0
#define A   1
#define B   2
#define FIRE 3
#define C   4

#define IDLE 0
#define A_ARRIVED 1
#define B_ARRIVED 2
#define A_AND_B_ARRIVED 3
#define OUTPUT 4

using namespace ta;

Automata * NAND_GATE() {
    Automata * res = new Automata();
    res -> appendNode(); // s_0: idle
    res -> appendNode(); // s_1: a_arrived
    res -> appendNode(); // s_2: b_arrived
    res -> appendNode(); // s_3: a_and_b_arrived
    res -> appendNode(); // s_4: fire

    res -> appendEdge(Edge(IDLE, OUTPUT, CLK, NO_OUTPUT));
    res -> appendEdge(Edge(IDLE, A_ARRIVED, A, NO_OUTPUT));
    res -> appendEdge(Edge(IDLE, B_ARRIVED, B, NO_OUTPUT));
    res -> appendEdge(Edge(IDLE, IDLE, FIRE, NO_OUTPUT)); 
    res -> appendEdge(Edge(A_ARRIVED, A_AND_B_ARRIVED, B, NO_OUTPUT));
    res -> appendEdge(Edge(A_ARRIVED, A_ARRIVED, A, NO_OUTPUT));
    res -> appendEdge(Edge(A_ARRIVED, OUTPUT, CLK, NO_OUTPUT));
    res -> appendEdge(Edge(A_ARRIVED, A_ARRIVED, FIRE, NO_OUTPUT));
    res -> appendEdge(Edge(B_ARRIVED, A_AND_B_ARRIVED, A, NO_OUTPUT));
    res -> appendEdge(Edge(B_ARRIVED, OUTPUT, CLK, NO_OUTPUT));
    res -> appendEdge(Edge(B_ARRIVED, B_ARRIVED, B, NO_OUTPUT));
    res -> appendEdge(Edge(B_ARRIVED, B_ARRIVED, FIRE, NO_OUTPUT));
    res -> appendEdge(Edge(A_AND_B_ARRIVED, IDLE, CLK, NO_OUTPUT));
    res -> appendEdge(Edge(A_AND_B_ARRIVED, A_AND_B_ARRIVED, A, NO_OUTPUT)); 
    res -> appendEdge(Edge(A_AND_B_ARRIVED, A_AND_B_ARRIVED, B, NO_OUTPUT));
    res -> appendEdge(Edge(A_AND_B_ARRIVED, A_AND_B_ARRIVED, FIRE, NO_OUTPUT));
    res -> appendEdge(Edge(OUTPUT, IDLE, FIRE, C));
    res -> full_reduce();

    return res;
}
