#include "automata.h"

int main() {
    Automata a;
    a.deserialize();
    int garbage = a.node();
    for (auto& node : a.nodes)
        for (int c = 0; c < W; c++)
            if (!node[c].size() && c != chrid('\e'))
                node[c].emplace(garbage);
    a.serialize();
}
