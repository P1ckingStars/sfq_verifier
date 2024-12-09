

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
  Clock c(0);
  auto df1 = builder.makeDFGate();
  auto df2 = builder.makeDFGate();
  auto df3 = builder.makeDFGate();
  auto df4 = builder.makeDFGate();
  auto df5 = builder.makeDFGate();
  auto df6 = builder.makeDFGate();
  auto not1 = builder.makeNOTGate();
  auto and1 = builder.makeANDGate();
  auto and2 = builder.makeANDGate();
  auto or1 = builder.makeORGate();
  auto inputA = df4.C();
  auto inputNotA = not1.C();
  auto inputB = df5.C();
  auto inputC = df6.C();
  builder.assign(df4.A(), df1.C());
  builder.assign(df5.A(), df2.C());
  builder.assign(df6.A(), df3.C());
  builder.assign(and1.A(), inputA);
  builder.assign(and1.B(), inputB);
  builder.assign(and2.A(), inputNotA);
  builder.assign(and2.B(), inputC);
  builder.assign(or1.A(), and1.C());
  builder.assign(or1.B(), and2.C());
  PulseCA *pca = builder.build(&c);
  auto dfa = pca->to_dfa();
  pca->print_map();
  dfa->print_transition_table();
    printf("REDUCE NOW\n");
  dfa->full_reduce();
  dfa->print_transition_table();
  return 0;
}

