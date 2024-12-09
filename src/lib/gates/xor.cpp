/*
class XOR(SFQ):
    ''' Sync. XOR element

    Default numbers come from [2].
    '''
    _setup_time = 3.7
    _hold_time = 4.1

    name = 'XOR'
    inputs = ['a', 'b', 'clk']
    outputs = ['q']
    transitions = [
        # Order if simultaneous is based on order of transitions given for a given source,
        # meaning here, while in 'idle', 'a' will be handled first, then 'b', then finally 'clk',
        # if they arrive simultaneously.
        {'id': '0', 'source': 'idle',      'trigger': 'a',   'dest': 'a_arrived'},
        {'id': '1', 'source': 'idle',      'trigger': 'b',   'dest': 'b_arrived'},
        {'id': '2', 'source': 'idle',      'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time}},

        {'id': '3', 'source': 'a_arrived', 'trigger': 'a',   'dest': 'a_arrived'},
        {'id': '4', 'source': 'a_arrived', 'trigger': 'b',   'dest': 'idle'},
        {'id': '5', 'source': 'a_arrived', 'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time},
         'firing': 'q'},

        {'id': '6', 'source': 'b_arrived', 'trigger': 'b',   'dest': 'b_arrived'},
        {'id': '7', 'source': 'b_arrived', 'trigger': 'a',   'dest': 'idle'},
        {'id': '8', 'source': 'b_arrived', 'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time},
         'firing': 'q'},
    ]
    jjs = 11
    firing_delay = 6.5
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

Automata * XOR_GATE() {
    Automata * res = new Automata();
    res->appendNode(); // s_0
    res->appendNode(); // s_1: a_arrive
    res->appendNode(); // s_2: b_arrive
    res->appendNode(); // s_3: ab_arrive

    res->appendEdge(Edge(IDLE, IDLE, CLK));
    res->appendEdge(Edge(IDLE, A_ARRIVED, A));
    res->appendEdge(Edge(IDLE, B_ARRIVED, B));
    res->appendEdge(Edge(A_ARRIVED, A_ARRIVED, A));
    res->appendEdge(Edge(A_ARRIVED, IDLE, B));
    res->appendEdge(Edge(A_ARRIVED, IDLE, CLK, C));
    res->appendEdge(Edge(B_ARRIVED, B_ARRIVED, B));
    res->appendEdge(Edge(B_ARRIVED, IDLE, A));
    res->appendEdge(Edge(B_ARRIVED, IDLE, CLK, C));
    res->full_reduce();
    return res;
}