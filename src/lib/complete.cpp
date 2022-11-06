#include "../automata.h"

void complete_dfa(Automata& dfa) {
    int garbage = dfa.node();
    bool used = false;
    for (int node_number = 0; node_number < dfa.size(); node_number++) {
        auto& node = dfa.nodes[node_number];
        for (int symbol = 0; symbol < dfa.max_literal; symbol++) {
            if (!node[symbol].size() && symbol != EPSILON) {
                node[symbol].emplace(garbage);
                used = used || node_number != garbage;
            }
        }
    }
    if (!used) {
        dfa.nodes.pop_back();
    }
}
