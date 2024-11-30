
#include "automata.hpp"

#define CLK 0
#define FIRE 1
#define A 0
#define CLK_STATE 1
#define FIRE_STATE 2

using namespace ta;

Automata *AND_GATE() {
  Automata *res = new Automata();
  res->appendNode(); // s_0: idle
  res->appendNode(); // s_1: a_arrived

  res->appendEdge(Edge(CLK_STATE, FIRE_STATE, A, CLK));
  res->appendEdge(Edge(FIRE_STATE, CLK_STATE, A, FIRE));
  return res;
}
