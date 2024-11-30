/*
class XNOR(SFQ):
    ''' Sync. XNOR element

    Default JJ numbers come from [4].
    '''
    _setup_time = 3.7
    _hold_time = 0.0

    name = 'XNOR'
    inputs = ['a', 'b', 'clk']
    outputs = ['q']
    transitions = [
        {'id': '0',  'source': 'idle',      'trigger': 'a',   'dest': 'a_arrived'},
        {'id': '1',  'source': 'idle',      'trigger': 'b',   'dest': 'b_arrived'},
        {'id': '2',  'source': 'idle',      'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time},
         'firing': 'q'},

        {'id': '3',  'source': 'a_arrived', 'trigger': 'a',   'dest': 'a_arrived'},
        {'id': '4',  'source': 'a_arrived', 'trigger': 'b',   'dest': 'a_and_b_arrived'},
        {'id': '5',  'source': 'a_arrived', 'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time}},

        {'id': '6',  'source': 'b_arrived', 'trigger': 'b',   'dest': 'b_arrived'},
        {'id': '7',  'source': 'b_arrived', 'trigger': 'a',   'dest': 'a_and_b_arrived'},
        {'id': '8',  'source': 'b_arrived', 'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time}},

        {'id': '9',  'source': 'a_and_b_arrived', 'trigger': 'b',   'dest': 'a_and_b_arrived'},
        {'id': '10', 'source': 'a_and_b_arrived', 'trigger': 'a',   'dest': 'a_and_b_arrived'},
        {'id': '11', 'source': 'a_and_b_arrived', 'trigger': 'clk', 'dest': 'idle',
         'transition_time': _hold_time, 'past_constraints': {'*': _setup_time},
         'firing': 'q'},
    ]
    jjs = 15
    firing_delay = 6.5
*/
#define CLK 0
#define A   1
#define B   2
#define C   3

#define IDLE 0
#define A_ARRIVE 1
#define B_ARRIVE 2
#define AB_ARRIVE 3
#define FIRE 4

using namespace ta;

Automata * XNOR_GATE() {
    Automata * res = new Automata();
    res -> appendNode(); // s_0
    res -> appendNode(); // s_1: a_arrive
    res -> appendNode(); // s_2: b_arrive
    res -> appendNode(); // s_3: ab_arrive
    res -> appendNode(); // s_4: fire_c
    
    res -> appendEdge(Edge(IDLE, OUTPUT, CLK));
    res -> appendEdge(Edge(IDLE, A_ARRIVED, A));
    res -> appendEdge(Edge(IDLE, B_ARRIVED, B));
    res -> appendEdge(Edge(A_ARRIVED, A_ARRIVED, A));
    res -> appendEdge(Edge(A_ARRIVED, AB_ARRIVE, B));
    res -> appendEdge(Edge(A_ARRIVED, IDLE, CLK));
    res -> appendEdge(Edge(B_ARRIVED, B_ARRIVED, B));
    res -> appendEdge(Edge(B_ARRIVED, AB_ARRIVE, A));
    res -> appendEdge(Edge(B_ARRIVED, IDLE, CLK));
    res -> appendEdge(Edge(AB_ARRIVE, OUTPUT, CLK));
    res -> appendEdge(Edge(AB_ARRIVE, AB_ARRIVE, A));
    res -> appendEdge(Edge(AB_ARRIVE, AB_ARRIVE, B));
    //res -> appendEdge(Edge(OUTPUT, IDLE, C));
    res -> full_reduce();
    return res;
}