

#include "automata.hpp"
#include "cta.hpp"
#include "gates.hpp"
#include <cstdio>
#include <iostream>
#include <vector>

using namespace ta;
using namespace std;

void and_test1(Automata *automata) {
  letter_t arr[] = {0, 1, 2, 0, 1, 0, 1, 2, 0, 2, 1, 0};
  vector<letter_t> in_seq = {arr, arr + (sizeof(arr) / sizeof(letter_t))};
  vector<letter_t> out_seq;
  automata->run(in_seq, out_seq);
  for (auto i : out_seq)
    cout << i << " ";
  cout << endl;
}

/*
 *
 * FULL
 *
 * */

int main() {
  ConnectAutomataBuilder builder;
  Clock c(0);

  auto dfa = builder.makeDFGate();
  auto dfb = builder.makeDFGate();
  auto dfc = builder.makeDFGate();
  auto dfd = builder.makeDFGate();

  auto xor1 = builder.makeXORGate();
  auto and1 = builder.makeANDGate();

  auto xor2 = builder.makeXORGate();
  auto and2 = builder.makeANDGate();

  auto or_gate = builder.makeORGate();

  builder.assign(xor1.A(), dfa.C());
  builder.assign(xor1.B(), dfb.C());
  builder.assign(and1.A(), dfa.C());
  builder.assign(and1.B(), dfb.C());

  builder.assign(dfd.A(), xor1.C());
  builder.assign(xor2.A(), dfd.C());
  builder.assign(xor2.B(), dfc.C());
  builder.assign(and2.A(), dfd.C());
  builder.assign(and2.B(), dfc.C());

  builder.assign(or_gate.A(), and1.C());
  builder.assign(or_gate.B(), and2.C());
  PulseCA *pca = builder.build(&c);
  auto adder = pca->to_dfa();
  pca->print_map();
  adder->print_transition_table();
    printf("REDUCE NOW\n");
  adder->hopcroft();
  adder->print_transition_table();
  return 0;
}

