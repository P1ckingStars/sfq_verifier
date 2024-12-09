
#include "automata.hpp"
#include "gates.hpp"

#define A 0
#define B 1
#define INIT_STATE 0

using namespace ta;

Automata * SYNC_UNIT() {
  Automata *res = new Automata();
  res->appendNode(); 
  res->appendEdge(Edge(INIT_STATE, INIT_STATE, A, B));
  return res;
}
