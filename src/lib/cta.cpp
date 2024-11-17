
#include "cta.hpp"
#include <sstream>

namespace ta {
string ta::PulseCAState::to_str() {
    stringstream ss;
    ss << "states: ";
    for (int i = 0; i < this->states.size(); i++) {
        ss << this->states[i] << " ";
    }
    ss << "\nchannel states: ";
    for (int i = 0; i < this->channel_states.size(); i++) {
        ss << this->channel_states[i] << " ";
    }
    return ss.str();
}

}
