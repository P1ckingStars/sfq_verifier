
#include "automata.hpp"
#include "timed_automata.hpp"
#include <cstddef>
#include <iostream>
#include <map>
#include <string>
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
  struct edge_cmp {
    state_t to;
    set<letter_t> out;
    bool operator<(edge_cmp const &p) const {
      return this->to < p.to || (this->to == p.to && this->out < p.out);
    }
  };
  map<edge_cmp, Edge> new_edges;
  for (Edge e : this->fwd_edges) {
    edge_cmp idx = {e.to, e.output};
    if (!new_edges.count(idx)) {
      new_edges[idx] = e;
    } else {
      for (letter_t act : e.letters) {
        new_edges[idx].letters.insert(act);
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
map<state_t, state_t> finalize_equivalant(eq_class &classes) {
  for (int i = 0; i < classes.size(); i++) {
    classes[i] = finalize(classes, i);
  }
  map<state_t, state_t> new_mapping;
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
  cout << "reduce" << endl;
  eq_class classes(this->states.size());
  init_equivalant(classes);
  unordered_map<string, state_id> find_same;
  find_same.clear();
  for (int i = 0; i < this->states.size() - 1; i++) {
    for (int j = i + 1; j < this->states.size(); j++) {
      if (this->states[i] == this->states[j]) {
        add_equivalant(classes, i, j);
      }
    }
  }
  /*
for (int i = 0; i < this->states.size() - 1; i++) {
  if (find_same.count(this->states[i].to_string_idx())) {
    add_equivalant(classes, i, find_same[this->states[i].to_string_idx()]);
  } else {
    find_same[this->states[i].to_string_idx()] = i;
  }
}
     */
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

letter_t Automata::replace_input(letter_t begin, map<letter_t, letter_t> &mp) {
  for (int i = 0; i < states.size(); i++) {
    set<Edge> new_edges;
    for (Edge e : this->states[i].fwd_edges) {
      Edge edge = e;
      set<letter_t> new_letters;
      for (letter_t l : e.letters) {
        if (!mp.count(l)) {
          mp.insert({l, begin});
          begin++;
        }
        new_letters.insert(mp[l]);
      }
      edge.letters = new_letters;
      set<letter_t> new_output;
      for (letter_t l : e.output) {
        if (!mp.count(l)) {
          mp.insert({l, begin});
          begin++;
        }
        new_output.insert(mp[l]);
      }
      edge.output = new_output;
      new_edges.insert(edge);
    }
    this->states[i].fwd_edges = new_edges;
  }
  return begin;
}

pair<state_t, set<letter_t>> Automata::next(state_t from, letter_t act) {
  for (Edge edge : this->states[from].fwd_edges) {
    if (edge.letters.count(act))
      return {edge.to, edge.output};
  }
  return {STATE_NOT_EXISTS, set<letter_t>()};
}

bool Automata::run(vector<letter_t> input, vector<letter_t> &full_seqeunce) {
  state_t s = 0;
  for (auto letter : input) {
    full_seqeunce.push_back(letter);
    auto p = this->next(s, letter);
    if (p.first == STATE_NOT_EXISTS)
      return false;
    s = p.first;
    if (!p.second.empty()) {
      for (auto l : p.second) {
        full_seqeunce.push_back(l);
      }
    }
  }
  return true;
}

} // namespace ta
