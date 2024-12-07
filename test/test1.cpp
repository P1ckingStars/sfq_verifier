

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
 * (a AND b) OR (!a AND c)
 *
 *
 * */

int main() {
  ConnectAutomataBuilder builder;
  auto df1 = builder.makeDFGate();
  auto df2 = builder.makeDFGate();
  auto df3 = builder.makeDFGate();
  auto not1 = builder.makeNOTGate();
  auto and1 = builder.makeANDGate();
  auto and2 = builder.makeANDGate();
  auto or1 = builder.makeORGate();
  builder.assign(and1.A(), df1.C());
  builder.assign(and1.B(), df2.C());
  builder.assign(and2.A(), df1.C());
  builder.assign(and2.B(), not1.C());
  builder.assign(or1.A(), and1.C());
  builder.assign(or1.B(), and2.C());
  PulseCA *pca = builder.build();
  auto dfa = pca->to_dfa();
  pca->print_map();
  dfa->print_transition_table();
    printf("REDUCE NOW\n");
  dfa->full_reduce();
  dfa->print_transition_table();
  return 0;
}

