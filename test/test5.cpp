

#include "automata.hpp"
#include "cta.hpp"
#include "gates.hpp"
#include <cstdio>
#include <iostream>
#include <vector>

using namespace ta;
using namespace std;

/*
 *
 * HALF
 *
 * */

int main() {
  ConnectAutomataBuilder builder;
  Clock c(0);
  auto df1 = builder.makeDFGate();
  auto df2 = builder.makeDFGate();
  auto xor_gate = builder.makeXORGate();
  auto and_gate = builder.makeANDGate();
  builder.assign(xor_gate.A(), df1.C());
  builder.assign(xor_gate.B(), df2.C());
  builder.assign(and_gate.A(), df1.C());
  builder.assign(and_gate.B(), df2.C());
  PulseCA *pca = builder.build(&c);
  auto dfa = pca->to_dfa();
  pca->print_map();
  dfa->print_transition_table();
    printf("REDUCE NOW\n");
  dfa->hopcroft();
  dfa->print_transition_table();
  return 0;
}

