#ifndef AUTOMATA
#define AUTOMATA

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

namespace ta {

#define NO_OUTPUT (1 << 31)

typedef uint32_t letter_t;
typedef uint32_t state_t;

using namespace std;

struct Edge {
  state_t from;
  state_t to;
  set<letter_t> letters;
  set<letter_t> output;
  Edge() {}
  Edge(state_t from, state_t to, letter_t l) {
    this->from = from;
    this->to = to;
    this->letters.insert(l);
  }
  Edge(state_t from, state_t to, letter_t l, set<letter_t> output) {
    this->from = from;
    this->to = to;
    this->output = output;
    this->letters.insert(l);
  }
  Edge(state_t from, state_t to, letter_t l, letter_t output) {
    this->from = from;
    this->to = to;
    this->output.insert(output);
    this->letters.insert(l);
  }
  // definition of equivalant classes
  bool operator==(Edge const &e) const {
    return this->to == e.to && this->letters == e.letters &&
           this->output == e.output;
  }
  // provide an operation for c++ standard set to search
  bool operator<(Edge const &e) const {
    if (this->to < e.to)
      return true;
    if (this->letters.size() < e.letters.size())
      return true;
    auto my_letter = this->letters.begin();
    auto e_letter = e.letters.begin();
    while (my_letter != this->letters.end()) {
      if (*my_letter == *e_letter)
        continue;
      return *my_letter < *e_letter;
    }
    return this->output < e.output;
  }
};
struct Node {
  state_t id;
  set<Edge> fwd_edges;
  Node() {}
  Node(state_t nodeid) { id = nodeid; }
  string to_string_idx() {
    stringstream ss;
    set<Edge> new_edge;
    for (auto e : fwd_edges) {
      if (e.to == this->id) {
        e.to = -1;
      }
      new_edge.insert(e);
    }
    for (auto e : new_edge) {
      ss << e.to << ": ";
      for (auto l : e.letters) {
        ss << l << ", ";
      }
      ss << ";";
    }
    return ss.str();
  }
  bool operator==(Node const &node) const {
    if (this->fwd_edges.size() != node.fwd_edges.size()) {
      return false;
    }
    set<Edge> my_edges;
    set<Edge> node_edges;
    for (auto e : fwd_edges) {
      if (e.to == node.id) {
        e.to = this->id;
      }
      my_edges.insert(e);
    }
    for (auto e : node.fwd_edges) {
      if (e.to == node.id) {
        e.to = this->id;
      }
      node_edges.insert(e);
    }
    return my_edges == node_edges;
  }
  void mergeEdge();
};

class Automata {
  uint32_t total_states;
  vector<Node> states;
  set<letter_t> acceptable;

public:
  //  0 1 2 3 4 5
  //
  //  suppose begin = 100
  //  100 101 102 103 104 105
  size_t num_states() { return states.size(); }
  letter_t replace_input(letter_t begin, map<letter_t, letter_t> &mp);
#define STATE_NOT_EXISTS (1 << 31)
  // return (1 << 31) if not exists
  pair<state_t, set<letter_t>> next(state_t from, letter_t act);
  void appendNode();
  void mergeEdge();
  void appendEdge(Edge e);
  size_t reduce();
  size_t full_reduce();
  size_t hopcroft();
  bool run(vector<letter_t> input, vector<letter_t> &full_seqeunce);
  void print_transition_table() {
    for (int i = 0; i < states.size(); i++) {
      cout << "STATE " << i << ": " << endl;
      for (auto edge : states[i].fwd_edges) {
        cout << "    ";
        for (letter_t act : edge.letters) {
          cout << " | " << act;
        }
        cout << " ==>> " << edge.to << " out: ";
        for (letter_t l : edge.output) {
          cout << l;
        }
        cout << endl;
      }
    }
  }
};

} // namespace ta

#endif
