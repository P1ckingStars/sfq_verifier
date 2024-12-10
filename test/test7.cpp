

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
 * (A and B) or (C and !D)
 *
 * */

int main() {
  ConnectAutomataBuilder builder;
  Clock c(0);
  auto df1 = builder.makeDFGate();
  auto df2 = builder.makeDFGate();
  auto df3 = builder.makeDFGate();
  auto not1 = builder.makeNOTGate();
  auto and1 = builder.makeANDGate();
  auto and2 = builder.makeANDGate();
  auto or1 = builder.makeORGate();
  auto inputA = df1.C();
  auto inputB = df2.C();
  auto inputC = df3.C();
  auto inputNotD = not1.C();
  builder.assign(and1.A(), inputA);
  builder.assign(and1.B(), inputB);
  builder.assign(and2.A(), inputC);
  builder.assign(and2.B(), inputNotD);
  builder.assign(or1.A(), and1.C());
  builder.assign(or1.B(), and2.C());
  PulseCA *pca = builder.build(&c);
  auto dfa = pca->to_dfa();
  pca->print_map();
  dfa->print_transition_table();
    printf("REDUCE NOW\n");
  dfa->hopcroft();
  dfa->print_transition_table();
  return 0;
}

