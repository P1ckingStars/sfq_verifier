

#include "automata.hpp"
#include "gates.hpp"

#define CLK 0
#define A   1
#define C   2


#define IDLE 0
#define A_ARRIVED 1

using namespace ta;

Automata * DF_GATE() {
    Automata * res = new Automata();
    res->appendNode(); // s_0
    res->appendNode(); // s_1: a_arrive

    res->appendEdge(Edge(IDLE, A_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(IDLE, IDLE, CLK, NO_OUTPUT));
    res->appendEdge(Edge(A_ARRIVED, A_ARRIVED, A, NO_OUTPUT));
    res->appendEdge(Edge(A_ARRIVED, IDLE, CLK, C));
    res -> full_reduce();
    return res;
}
