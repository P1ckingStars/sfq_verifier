

#include "automata.hpp"
#include "cta.hpp"
#include "gates.hpp"
#include <iostream>
#include <vector>

using namespace ta;
using namespace std;

// 3 XOR GATE //

int main() {
  ConnectAutomataBuilder builder;
  Clock c(0);
  auto xor1 = builder.makeXORGate();
  auto xor2 = builder.makeXORGate();
  auto xor3 = builder.makeXORGate();
  builder.assign(xor3.A(), xor1.C());
  builder.assign(xor3.B(), xor2.C());
  PulseCA *pca = builder.build(&c);
  auto dfa = pca->to_dfa();
  pca->print_map();
  dfa->print_transition_table();
  printf("REDUCE NOW\n");
  dfa->hopcroft();
  dfa->print_transition_table();
  return 0;
}
