#include "automata.hpp"
#include <cstddef>
#include <map>
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

void Node::mergeEdge() {
  map<state_t, Edge> new_edges;
  for (Edge e : this->fwd_edges) {
    if (!new_edges.count(e.to)) {
      new_edges[e.to] = e;
    } else {
      for (letter_t act : e.letters) {
        new_edges[e.to].letters.insert(act);
      }
    }
  }
  this->fwd_edges.clear();
  for (auto e : new_edges) {
    this->fwd_edges.insert(e.second);
  }
}

void Automata::mergeEdge() {
  for (Node n : this->states) {
    n.mergeEdge();
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
    set<Edge> new_fwd_edges;

    for (Edge e : node.fwd_edges) {
      e.to = classes[e.to];
      e.from = classes[e.from];
      new_fwd_edges.insert(e);
    }
    node.fwd_edges = new_fwd_edges;
  }
  for (auto pair : new_mapping) {
    this->states[pair.second] = this->states[pair.first];
  }
  this->states.resize(new_mapping.size());
  this->mergeEdge();
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

void Automata::appendEdge(Edge e) { this->states[e.from].fwd_edges.insert(e); }

letter_t Automata::replace_input(letter_t begin,
                                 unordered_map<letter_t, letter_t> &mp) {
  for (Node n : this->states) {
    for (Edge edge : n.fwd_edges) {
      unordered_set<letter_t> new_letters;
      for (letter_t l : edge.letters) {
        if (!mp.count(l)) {
          mp.insert({l, begin});
          begin++;
        }
        new_letters.insert(mp[l]);
      }
      edge.letters = new_letters;
    }
  }
  return begin;
}

pair<state_t, letter_t> Automata::next(state_t from, letter_t act) {
  for (Edge edge : this->states[from].fwd_edges) {
    if (edge.letters.count(act))
      return {edge.to, edge.output};
  }
  return {STATE_NOT_EXISTS, 0};
}

bool Automata::run(vector<letter_t> input, vector<letter_t> & full_seqeunce) {
  state_t s = 0;
  for (auto letter : input) {
    full_seqeunce.push_back(letter);
    auto p = this->next(s, letter);
    if (p.first == STATE_NOT_EXISTS) return false;
    s = p.first;
    if (p.second != NO_OUTPUT) full_seqeunce.push_back(p.second);
  }
  return true;
}

} // namespace ta
