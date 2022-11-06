#include "../automata.h"

void inverse_cdfa(Automata& cdfa) {
    std::vector<bool> is_terminate(cdfa.size());
    for (int node : cdfa.final_states) {
        is_terminate[node] = 1;
    }
    cdfa.final_states = {};
    for (int node = 0; node < is_terminate.size(); node++) {
        if (!is_terminate[node]) {
            cdfa.final_states.emplace(node);
        }
    }
}
