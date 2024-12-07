#include "automata.hpp"
#include "gates.hpp"

#define CLK 0
#define A   1
#define B   2
#define SEL 3
#define C   4

#define IDLE 0
#define A_ARRIVED 1
#define B_ARRIVED 2
#define SEL_ARRIVED 3
#define B_AND_SEL_ARRIVED 4
#define A_AND_B_ARRIVED 5

using namespace ta;

Automata * AND_GATE() {
    Automata * res = new Automata();
    res->appendNode(); // s_0: idle
    res->appendNode(); // s_1: a_arrived
    res->appendNode(); // s_2: b_arrived
    res->appendNode(); // s_3: a_and_b_arrived
    res->appendNode();
    res->appendNode();
    
    res->appendEdge(Edge(IDLE, IDLE, CLK, NO_OUTPUT));
    res->appendEdge(Edge(IDLE, A_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(IDLE, B_ARRIVED, B, NO_OUTPUT));
    res->appendEdge(Edge(IDLE, SEL_ARRIVED, SEL, NO_OUTPUT));
    res->appendEdge(Edge(A_ARRIVED, A_AND_B_ARRIVED, B, NO_OUTPUT));
    res->appendEdge(Edge(A_ARRIVED, A_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(A_ARRIVED, SEL_ARRIVED, SEL, NO_OUTPUT));
    res->appendEdge(Edge(A_ARRIVED, IDLE, CLK, C));
    res->appendEdge(Edge(B_ARRIVED, A_AND_B_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(B_ARRIVED, IDLE, CLK, NO_OUTPUT));
    res->appendEdge(Edge(B_ARRIVED, B_ARRIVED, B, NO_OUTPUT));
    res->appendEdge(Edge(B_ARRIVED, B_AND_SEL_ARRIVED, SEL, NO_OUTPUT));
    res->appendEdge(Edge(A_AND_B_ARRIVED, IDLE, CLK, C));
    res->appendEdge(Edge(A_AND_B_ARRIVED, A_AND_B_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(A_AND_B_ARRIVED, A_AND_B_ARRIVED, B, NO_OUTPUT));
    res->appendEdge(Edge(A_AND_B_ARRIVED, B_AND_SEL_ARRIVED, SEL, NO_OUTPUT));
    res->appendEdge(Edge(SEL_ARRIVED, SEL_ARRIVED, SEL, NO_OUTPUT));
    res->appendEdge(Edge(SEL_ARRIVED, SEL_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(SEL_ARRIVED, IDLE, CLK, NO_OUTPUT));
    res->appendEdge(Edge(SEL_ARRIVED, B_AND_SEL_ARRIVED, B, NO_OUTPUT));
    res->appendEdge(Edge(B_AND_SEL_ARRIVED, B_AND_SEL_ARRIVED, SEL, NO_OUTPUT));
    res->appendEdge(Edge(B_AND_SEL_ARRIVED, B_AND_SEL_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(B_AND_SEL_ARRIVED, B_AND_SEL_ARRIVED, B, NO_OUTPUT));
    res->appendEdge(Edge(B_AND_SEL_ARRIVED, IDLE, CLK, C));
    res->full_reduce();

    return res;
}

