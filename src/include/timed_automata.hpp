#ifndef TIMED_AUTOMATA
#define TIMED_AUTOMATA

#include <cmath>
#include <cstdint>
#include <set>
#include <unordered_map>
#include <vector>

using namespace std;

namespace ta {

typedef int clock_id;
typedef int state_id;
typedef int input_id;
typedef int output_id;

class Constraints {
public:
    virtual bool check_clock(float x) = 0;
};

class UnionContraints : Constraints {
    Constraints * lhs;
    Constraints * rhs;
public:
    bool check_clock(float x) override {
        return lhs->check_clock(x) && rhs->check_clock(x);
    }
};

class NotContraints : Constraints {
    Constraints * c;
public:
    bool check_clock(float x) override {
        return !c->check_clock(x);
    }
};
class LBContraints : Constraints {
    bool exclusive;
    int val;
public:
    bool check_clock(float x) override {
        return (x > val) || (exclusive && (x == val));
    }
};

class UBContraints : Constraints {
    bool exclusive;
    int val;
public:
    bool check_clock(float x) override {
        return (x < val) || (exclusive && (x == val));
    }
};

struct Edge {
    state_id from_state;
    state_id to_state;
    set<set<input_id>> inputs;
    set<output_id> outputs;
    unordered_map<int, Constraints *> constraints;
    vector<int> clocks;
};

class TransitionTable {
    uint64_t num_states_;
    uint64_t num_clocks_;
    vector<vector<Edge>> table_;
};

}









#endif
