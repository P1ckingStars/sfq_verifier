
#include "automata.hpp"
#include "gates.hpp"

#define FIRE 0
#define A 1
#define B 2

#define IDLE 0
#define PULSE 1

using namespace ta;

Automata *WIRE() {
  Automata *res = new Automata();
  res->appendNode(); // s_0: idle
  res->appendNode(); // s_1: output arrived
  res->appendEdge(Edge(IDLE, PULSE, A, NO_OUTPUT));
  res->appendEdge(Edge(IDLE, IDLE, FIRE, NO_OUTPUT));
  res->appendEdge(Edge(PULSE, IDLE, FIRE, B));
  return res;
}
