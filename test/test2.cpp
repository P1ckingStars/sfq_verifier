

#include "automata.hpp"
#include "cta.hpp"
#include "gates.hpp"
#include <iostream>
#include <vector>

using namespace ta;
using namespace std;

int main() {
  ConnectAutomataBuilder builder;
  auto or1 = builder.makeANDGate();
  auto or2 = builder.makeANDGate();
  auto or3 = builder.makeORGate();
  builder.assign(or3.A(), or1.C());
  builder.assign(or3.B(), or2.C());
  PulseCA *pca = builder.build();
  auto dfa = pca->to_dfa();
  pca->print_map();
  dfa->print_transition_table();
  printf("REDUCE NOW\n");
  dfa->full_reduce();
  dfa->print_transition_table();
  return 0;
}
