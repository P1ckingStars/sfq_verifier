#ifndef GATES_HPP
#define GATES_HPP

#include "automata.hpp"
#include "cta.hpp"
#include <map>
#include <utility>
#include <vector>
using namespace ta;

Automata *WIRE();
Automata *AND_GATE();
Automata *CLK_UNIT();
Automata *NAND_GATE();
Automata *NOR_GATE();
Automata *NOT_GATE();
Automata *DF_GATE();
Automata *OR_GATE();
Automata *XNOR_GATE();
Automata *XOR_GATE();

struct GateInterface {
  Automata *my_obj;
  automata_id id;
  GateInterface(automata_id id) { this->id = id; }
  Automata *automata() { return my_obj; }
};

struct Clock : GateInterface {
  Clock(automata_id id) : GateInterface(id) { this->my_obj = CLK_UNIT(); }
  pulse CLK() { return {id, 0}; }
  pulse FIRE() { return {id, 1}; }
  pulse A() { return {id, 2}; }
};

struct Wire : GateInterface {
  Wire(automata_id id) : GateInterface(id) { this->my_obj = WIRE(); }
  pulse FIRE() { return {id, 0}; }
  pulse A() { return {id, 1}; }
  pulse B() { return {id, 2}; }
};

struct ANDGate : GateInterface {
  ANDGate(automata_id id) : GateInterface(id) { this->my_obj = AND_GATE(); }
  pulse CLK() { return {id, 0}; }
  pulse A() { return {id, 1}; }
  pulse B() { return {id, 2}; }
  pulse C() { return {id, 3}; }
};

struct NANDGate : GateInterface {
  NANDGate(automata_id id) : GateInterface(id) { this->my_obj = NAND_GATE(); }
  pulse CLK() { return {id, 0}; }
  pulse A() { return {id, 1}; }
  pulse B() { return {id, 2}; }
  pulse C() { return {id, 3}; }
};

struct ORGate : GateInterface {
  ORGate(automata_id id) : GateInterface(id) { this->my_obj = OR_GATE(); }
  pulse CLK() { return {id, 0}; }
  pulse A() { return {id, 1}; }
  pulse B() { return {id, 2}; }
  pulse C() { return {id, 3}; }
};

struct NORGate : GateInterface {
  NORGate(automata_id id) : GateInterface(id) { this->my_obj = NOR_GATE(); }
  pulse CLK() { return {id, 0}; }
  pulse A() { return {id, 1}; }
  pulse B() { return {id, 2}; }
  pulse C() { return {id, 3}; }
};

struct XORGate : GateInterface {
  XORGate(automata_id id) : GateInterface(id) { this->my_obj = XOR_GATE(); }
  pulse CLK() { return {id, 0}; }
  pulse A() { return {id, 1}; }
  pulse B() { return {id, 2}; }
  pulse C() { return {id, 3}; }
};

struct XNORGate : GateInterface {
  XNORGate(automata_id id) : GateInterface(id) { this->my_obj = XNOR_GATE(); }
  pulse CLK() { return {id, 0}; }
  pulse A() { return {id, 1}; }
  pulse B() { return {id, 2}; }
  pulse C() { return {id, 3}; }
};

struct NOTGate : GateInterface {
  NOTGate(automata_id id) : GateInterface(id) { this->my_obj = NOT_GATE(); }
  pulse CLK() { return {id, 0}; }
  pulse A() { return {id, 1}; }
  pulse C() { return {id, 2}; }
};

struct DFGate : GateInterface {
  DFGate(automata_id id) : GateInterface(id) { this->my_obj = DF_GATE(); }
  pulse CLK() { return {id, 0}; }
  pulse A() { return {id, 1}; }
  pulse C() { return {id, 2}; }
};

class ConnectAutomataBuilder {
  vector<Automata *> automatas;
  vector<GateInterface> Gates;
  set<pulse> outputs;
  vector<PulseChannel *> channels;
  vector<pair<pulse, pulse>> edge;

public:
  ANDGate makeANDGate() {
    auto res = ANDGate(this->automatas.size());
    this->automatas.push_back(res.my_obj);
    Gates.push_back(res);
    outputs.insert(res.C());
    return res;
  }

  NANDGate makeNANDGate() {
    auto res = NANDGate(this->automatas.size());
    this->automatas.push_back(res.my_obj);
    Gates.push_back(res);
    outputs.insert(res.C());
    return res;
  }

  ORGate makeORGate() {
    auto res = ORGate(this->automatas.size());
    this->automatas.push_back(res.my_obj);
    Gates.push_back(res);
    outputs.insert(res.C());
    return res;
  }

  NORGate makeNORGate() {
    auto res = NORGate(this->automatas.size());
    this->automatas.push_back(res.my_obj);
    Gates.push_back(res);
    outputs.insert(res.C());
    return res;
  }

  XORGate makeXORGate() {
    auto res = XORGate(this->automatas.size());
    this->automatas.push_back(res.my_obj);
    Gates.push_back(res);
    outputs.insert(res.C());
    return res;
  }

  XNORGate makeXNORGate() {
    auto res = XNORGate(this->automatas.size());
    this->automatas.push_back(res.my_obj);
    Gates.push_back(res);
    outputs.insert(res.C());
    return res;
  }

  NOTGate makeNOTGate() {
    auto res = NOTGate(this->automatas.size());
    this->automatas.push_back(res.my_obj);
    Gates.push_back(res);
    outputs.insert(res.C());
    return res;
  }

  DFGate makeDFGate() {
    auto res = DFGate(this->automatas.size());
    this->automatas.push_back(res.my_obj);
    Gates.push_back(res);
    outputs.insert(res.C());
    return res;
  }

  void assign(pulse a, pulse b) { edge.push_back({b, a}); }

  PulseCA *build() {
    map<pulse, set<pulse>> wire_gen;
    set<pulse> dup_check;
    set<pulse> all_output = this->outputs;

    for (auto e : edge) {
      wire_gen[e.first].insert(e.second);
      if (all_output.count(e.first)) {
        all_output.erase(e.first);
      }
      if (dup_check.count(e.second)) {
        printf("ERROR duplicated assignments\n");
      }
    }
    automata_id wire_begin = automatas.size();
    for (auto p : wire_gen) {
      PulseChannel *channel = new PulseChannel();
      channel->in = p.first;
      for (auto to : p.second) {
        auto wire = Wire(this->automatas.size());
        this->automatas.push_back(wire.my_obj);
        channel->out.insert(wire.A());
        PulseChannel *out_channel = new PulseChannel();
        out_channel->in = wire.B();
        out_channel->out.insert(to);
        this->channels.push_back(out_channel);
      }
      this->channels.push_back(channel);
    }
    for (auto p : all_output) {
      auto wire = Wire(this->automatas.size());
      PulseChannel *channel = new PulseChannel();
      channel->in = p;
      channel->out.insert(wire.A());
      this->automatas.push_back(wire.my_obj);
      this->channels.push_back(channel);
    }
    Clock c = Clock(this->automatas.size());
    PulseChannel *clk_channel = new PulseChannel();
    clk_channel->in = c.CLK();
    for (automata_id i = 0; i < wire_begin; i++) {
      clk_channel->out.insert({i, 0});
    }
    this->channels.push_back(clk_channel);
    PulseChannel *fire_channel = new PulseChannel();
    for (automata_id i = wire_begin; i < automatas.size(); i++) {
      fire_channel->in = c.FIRE();
      fire_channel->out.insert({i, 0});
    }
    this->channels.push_back(fire_channel);
    this->automatas.push_back(c.my_obj);
    return new PulseCA(this->automatas, this->channels);
  }
};

#endif
