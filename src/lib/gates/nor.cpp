/*

class NOR(SFQ):
    ''' Sync. NOR element

    Default JJ numbers come from [4].
    '''
    _setup_time = 5.0
    _hold_time = 0.0

    name = 'NAND'
    inputs = ['a', 'b', 'clk']
    outputs = ['q']
    transitions = [
        {'id': '0',  'source': 'idle',      'trigger': 'a',   'dest': 'a_or_b_arrived'},
        {'id': '1',  'source': 'idle',      'trigger': 'b',   'dest': 'a_or_b_arrived'},
        {'id': '2',  'source': 'idle',      'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time},
         'firing': 'q'},

        {'id': '3',  'source': 'a_or_b_arrived', 'trigger': 'a',   'dest': 'a_or_b_arrived'},
        {'id': '4',  'source': 'a_or_b_arrived', 'trigger': 'b',   'dest': 'a_or_b_arrived'},
        {'id': '5',  'source': 'a_or_b_arrived', 'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time}},
    ]
    jjs = 12
    firing_delay = 5.0

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

Automata * NOR_Gate () {
    Automata * res = new Automata();
    res -> appendNode(); //IDLE
    res -> appendNode(); //A_OR_B_ARRIVED
    
    res -> appendEdge(Edge(IDLE, IDLE, CLK, C));
    res -> appendEdge(Edge(IDLE, A_OR_B_ARRIVED, A));
    res -> appendEdge(Edge(IDLE, A_OR_B_ARRIVED, B));
    res -> appendEdge(Edge(A_OR_B_ARRIVED, A_OR_B_ARRIVED, A));
    res -> appendEdge(Edge(A_OR_B_ARRIVED, A_OR_B_ARRIVED, B));
    res -> appendEdge(Edge(A_OR_B_ARRIVED, IDLE, CLK));
    res -> full_reduce();

    return res;
}