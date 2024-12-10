
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
  for (int i = 0; i < this->states.size() - 1; i++) {
    for (int j = i + 1; j < this->states.size(); j++) {
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

template <class T> set<T> set_minus(set<T> x, set<T> y) {
  set<T> res;
  for (auto l : x) {
    if (!y.count(l)) {
      res.insert(l);
    }
  }
  return res;
}

template <class T> set<T> intersect(set<T> const &x, set<T> const &y) {
  set<T> res;
  for (auto l : x) {
    if (y.count(l)) {
      res.insert(l);
    }
  }
  return res;
}

size_t Automata::hopcroft() {
  set<state_id> F;
  set<state_id> Q_F;
  set<set<state_id>> P;
  set<set<state_id>> W;
  for (int i = 0; i < this->states.size(); i++) {
    bool inserted = false;
    for (auto e : this->states[i].fwd_edges) {
      if (!e.output.empty()) {
        F.insert(i);
        inserted = true;
      }
    }
    if (!inserted)
      Q_F.insert(i);
  }
  P.insert(F);
  P.insert(Q_F);
  W.insert(F);
  W.insert(Q_F);
  while (!W.empty()) {
    set<state_id> A = *W.begin();
    W.erase(W.begin());
    for (auto l : this->acceptable) {
      set<state_id> X;
      X.clear();
      for (int i = 0; i < this->states.size(); i++) {
        auto n = this->next(i, l);
        if (A.count(n.first)) {
          X.insert(i);
        }
      }
      // finish set up X
      set<set<state_id>> newP;
      for (auto Y : P) {
        auto XnY = intersect(X, Y);
        auto Y_X = set_minus(Y, X);
        newP.insert(XnY);
        newP.insert(Y_X);
        if (XnY.empty() || Y_X.empty())
          continue;
        if (W.count(Y)) {
          W.erase(Y);
        }
        if (XnY.size() <= Y_X.size()) {
          W.insert(XnY);
        } else {
          W.insert(Y_X);
        }
      }
      P = newP;
    }
  }
  int idx = 1;
  map<state_id, state_id> classes;
  map<state_id, state_id> new_mapping;
  new_mapping[0] = 0;
  for (auto p : P) {
    if (p.count(0)) {
      for (auto s : p) {
        classes[s] = 0;
      }
    } else {
      for (auto s : p) {
        classes[s] = idx;
      }
      new_mapping[idx] = *p.begin();
      idx++;
    }
  }
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
  vector<Node> new_states(new_mapping.size());
  for (auto pair : new_mapping) {
    new_states[pair.first] = this->states[pair.second];
  }
  this->states = new_states;
  return full_reduce();
}

void Automata::appendNode() {
  this->states.push_back(Node(this->states.size()));
}

void Automata::appendEdge(Edge e) {
  this->states[e.from].fwd_edges.insert(e);
  for (auto l : e.letters) {
    this->acceptable.insert(l);
  }
}

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
