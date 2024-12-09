

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
  ConnectAutomataBuilder builder1;
  Clock c1(0);
  auto in_df1 = builder1.makeDFGate();
  auto in_df2 = builder1.makeDFGate();
  auto in_df3 = builder1.makeDFGate();
  PulseCA *pca1 = builder1.build(&c1);
  auto dfa1 = pca1->to_dfa();
  auto map1 = pca1->pulse2letter_map();
  pca1->print_map();
  dfa1->print_transition_table();
  printf("REDUCE NOW\n");
  dfa1->full_reduce();
  dfa1->print_transition_table();

  ConnectAutomataBuilder builder2;
  Clock c2(0);
  auto df1 = builder2.makeDFGate();
  auto df2 = builder2.makeDFGate();
  auto df3 = builder2.makeDFGate();
  auto not1 = builder2.makeNOTGate();
  auto and1 = builder2.makeANDGate();
  auto and2 = builder2.makeANDGate();
  auto or1 = builder2.makeORGate();
  builder2.assign(and1.A(), df1.C());
  builder2.assign(and1.B(), df2.C());
  builder2.assign(and2.A(), df1.C());
  builder2.assign(and2.B(), not1.C());
  builder2.assign(or1.A(), and1.C());
  builder2.assign(or1.B(), and2.C());
  PulseCA *pca2 = builder2.build(&c2);
  auto dfa2 = pca2->to_dfa();
  auto map2 = pca2->pulse2letter_map();
  pca2->print_map();
  dfa2->print_transition_table();
  printf("REDUCE NOW\n");
  dfa2->full_reduce();
  dfa2->print_transition_table();
    
    /*
  ConnectAutomataBuilder builder3;
  Clock c3(0);
  auto module1 = builder3.add_module(dfa1, map1[c1.A()]);
  auto module2 = builder3.add_module(dfa2, map2[c2.A()]);
  builder3.assign({module2, map2[df1.A()]}, {module1, map1[in_df1.C()]});
  builder3.assign({module2, map2[df2.A()]}, {module1, map1[in_df2.C()]});
  builder3.assign({module2, map2[df3.A()]}, {module1, map1[in_df3.C()]});
  builder3.assign({module2, map2[not1.A()]}, {module1, map1[in_df1.C()]});
  auto pca3 = builder3.build_sync(&c3);
  auto dfa3 = pca3->to_dfa();
  auto map3 = pca3->pulse2letter_map();
  pca3->print_map();
  dfa3->print_transition_table();
  printf("REDUCE NOW\n");
  dfa3->full_reduce();
  dfa3->print_transition_table();
    */
  return 0;
}
