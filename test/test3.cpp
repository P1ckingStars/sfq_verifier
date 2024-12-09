

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
  auto and1 = builder1.makeANDGate();
  PulseCA *pca1 = builder1.build_module(&c1);
  auto and_dfa1 = pca1->to_dfa();
  auto mp1 = pca1->pulse2letter_map();
  printf("REDUCE NOW\n");
  and_dfa1->full_reduce();

  ConnectAutomataBuilder builder2;
  Clock c2(0);
  auto and2 = builder2.makeANDGate();
  PulseCA *pca2 = builder2.build_module(&c2);
  auto and_dfa2 = pca2->to_dfa();
  auto mp2 = pca2->pulse2letter_map();
  printf("REDUCE NOW\n");
  and_dfa2->full_reduce();

  ConnectAutomataBuilder builder3;
  Clock c3(0);
  auto or3 = builder3.makeANDGate();
  PulseCA *pca3 = builder3.build_module(&c3);
  auto or_dfa3 = pca3->to_dfa();
  auto mp3 = pca3->pulse2letter_map();
  printf("REDUCE NOW\n");
  or_dfa3->full_reduce();

  ConnectAutomataBuilder builder4;
  Clock c4(0);
  auto and1_module = builder4.add_module(and_dfa1, mp1[c1.A()]);
  auto and2_module = builder4.add_module(and_dfa2, mp2[c2.A()]);
  auto or3_module = builder4.add_module(or_dfa3, mp3[c3.A()]);
  builder4.assign({ or3_module, mp3[or3.A()] } , { and1_module, mp1[and1.C()]});
  builder4.assign({ or3_module, mp3[or3.A()] } , { and2_module, mp2[and2.C()]});
  PulseCA *pca4 = builder4.build_sync(&c4);
  auto dfa4 = pca4->to_dfa();
  auto mp4 = pca4->pulse2letter_map();
  printf("REDUCE NOW\n");
  dfa4->full_reduce();
  return 0;
}
