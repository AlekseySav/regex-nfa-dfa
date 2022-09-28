#include "automata.h"

int main() {
    Automata a;
    a.deserialize();
    int garbage = a.node();
    for (int i = 0; i < a.size(); i++)
        for (int c = 0; c < W; c++)
            if (!a.nodes[i][c].size() && c != chrid('\e'))
                a.nodes[i][c].emplace(garbage);
    a.serialize();
}
