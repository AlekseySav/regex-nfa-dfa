#include "automata.h"

int main() {
    Automata input;
    input.deserialize();
    int garbage = input.node();
    bool used = false;
    for (int node_number = 0; node_number < input.size(); node_number++) {
        auto& node = input.nodes[node_number];
        for (int symbol = 0; symbol < input.max_literal; symbol++) {
            if (!node[symbol].size() && symbol != char_id('\e')) {
                node[symbol].emplace(garbage);
                used = used || node_number != garbage;
            }
        }
    }
    if (!used) input.nodes.pop_back();
    input.serialize();
}
