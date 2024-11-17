#ifndef AUTOMATA
#define AUTOMATA

#include "timed_automata.hpp"
#include <cstdint>
#include <forward_list>
#include <memory>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ta {

typedef uint32_t letter_t;
typedef uint32_t state_t;

using namespace std;

struct Edge {
  state_t from;
  state_t to;
  unordered_set<letter_t> letters;
  // definition of equivalant classes
  bool operator==(Edge const &e) const {
    return this->to == e.to && this->letters == e.letters;
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
    return this->to < e.to;
  }
};
struct Node {
  state_t id;
  set<Edge> fwd_edges;
  set<Edge> bwd_edges;
  Node(state_t nodeid) { id = nodeid; }
  bool operator==(Node const &node) const {
    return this->fwd_edges == node.fwd_edges;
  }
};

class Automata {
  uint32_t total_states;
  vector<Node> states;

public:
    //  0 1 2 3 4 5
    //
    //  suppose begin = 100
    //  100 101 102 103 104 105
  letter_t replace_input(letter_t begin, unordered_map<letter_t, letter_t> & mp);
  void appendNode();
  void appendEdge(Edge e);
  size_t reduce();
  size_t full_reduce();
  bool run(vector<letter_t> word);
};

} // namespace ta

#endif
