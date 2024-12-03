
#include "automata.hpp"
#include "gates.hpp"

#define CLK 0
#define FIRE 1
#define A 2
#define CLK_STATE 0
#define FIRE_STATE 1

using namespace ta;

Automata * CLK_UNIT() {
  Automata *res = new Automata();
  res->appendNode(); // s_0: idle
  res->appendNode(); // s_1: a_arrived

  res->appendEdge(Edge(CLK_STATE, FIRE_STATE, A, CLK));
  res->appendEdge(Edge(FIRE_STATE, CLK_STATE, A, FIRE));
  return res;
}
