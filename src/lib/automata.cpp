#include "automata.hpp"
#include <cstddef>
#include <unordered_map>

using namespace std;
namespace ta {

typedef vector<state_t> eq_class;

// union find algorithm, because union find is used to generate disjoint set, so
// we can us it.

// init
void init_equivalant(eq_class &classes) {
  for (int i = 0; i < classes.size(); i++) {
    classes[i] = i;
  }
}

// every element points to its class id
state_t finalize(eq_class &classes, int i) {
  return classes[i] == i ? i : finalize(classes, classes[i]);
}

// make a map from 1 to k for k equivalant classes
unordered_map<state_t, state_t> finalize_equivalant(eq_class &classes) {
  for (int i = 0; i < classes.size(); i++) {
    classes[i] = finalize(classes, i);
  }
  unordered_map<state_t, state_t> new_mapping;
  state_t new_state = 0;
  for (state_t i = 0; i < classes.size(); i++) {
    if (classes[i] == i)
      new_mapping[i] = new_state++;
  }
  for (int i = 0; i < classes.size(); i++) {
    classes[i] = new_mapping[classes[i]];
  }
  return new_mapping;
}

// union
void add_equivalant(eq_class &classes, state_t i, state_t j) { classes[j] = i; }

// collapse states for one step
size_t Automata::reduce() {
  eq_class classes(this->states.size());
  init_equivalant(classes);
  for (int i = 0; i < this->states.size() - 1; i++) {
    for (int j = 0; i < this->states.size(); j++) {
      if (this->states[i] == this->states[j]) {
        add_equivalant(classes, i, j);
      }
    }
  }
  auto new_mapping = finalize_equivalant(classes);
  for (int i = 0; i < this->states.size(); i++) {
    Node &node = this->states[i];
    node.id = classes[node.id];
    set<Edge> new_bwd_edges;
    set<Edge> new_fwd_edges;
    for (Edge e : node.bwd_edges) {
      e.to = classes[e.to];
      e.from = classes[e.from];
      new_bwd_edges.insert(e);
    }
    for (Edge e : node.fwd_edges) {
      e.to = classes[e.to];
      e.from = classes[e.from];
      new_fwd_edges.insert(e);
    }
    node.bwd_edges = new_bwd_edges;
    node.fwd_edges = new_fwd_edges;
  }
  for (auto pair : new_mapping) {
    this->states[pair.second] = this->states[pair.first];
  }
  this->states.resize(new_mapping.size());
  return new_mapping.size();
}

// collapse states until no more states to collapse
size_t Automata::full_reduce() {
  size_t new_size;
  for (size_t size = this->states.size(); size != (new_size = reduce());
       size = new_size)
    ;
  return this->states.size();
}

void Automata::appendNode() {
  this->states.push_back(Node(this->states.size()));
}

void Automata::appendEdge(Edge e) {
  this->states[e.from].fwd_edges.insert(e);
  this->states[e.to].bwd_edges.insert(e);
}
letter_t Automata::replace_input(letter_t begin, unordered_map<letter_t, letter_t> & mp) {}
} // namespace ta
