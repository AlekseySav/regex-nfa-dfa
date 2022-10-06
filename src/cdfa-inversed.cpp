#include "automata.h"

int main() {
    Automata input;
    input.deserialize();

    std::vector<bool> is_terminate(input.size());
    for (int node : input.final_states) {
        is_terminate[node] = 1;
    }
    input.final_states = {};
    for (int node = 0; node < is_terminate.size(); node++) {
        if (!is_terminate[node]) {
            input.final_states.emplace(node);
        }
    }

    input.serialize();
}
