#include "automata.h"

int main() {
    Automata a;
    a.deserialize();
    int garbage = a.node();
    bool used = false;
    for (int n = 0; n < a.size(); n++) {
        auto& node = a.nodes[n];
        for (int c = 0; c < a.max_literal; c++) {
            if (!node[c].size() && c != chrid('\e')) {
                node[c].emplace(garbage);
                used = used || n != garbage;
            }
        }
    }
    if (!used) a.nodes.pop_back();
    a.serialize();
}
