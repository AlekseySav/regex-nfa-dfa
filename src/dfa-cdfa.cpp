#include "automata.h"

int main() {
    Automata input;
    input.deserialize();
    int garbage = input.node();
    bool used = false;
    for (int n = 0; n < input.size(); n++) {
        auto& node = input.nodes[n];
        for (int c = 0; c < input.max_literal; c++) {
            if (!node[c].size() && c != chrid('\e')) {
                node[c].emplace(garbage);
                used = used || n != garbage;
            }
        }
    }
    if (!used) input.nodes.pop_back();
    input.serialize();
}
