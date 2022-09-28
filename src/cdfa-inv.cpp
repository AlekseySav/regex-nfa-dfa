#include "automata.h"

int main() {
    Automata a;
    a.deserialize();

    std::vector<bool> Q(a.size());
    for (int i : a.F)
        Q[i] = 1;
    a.F = {};
    for (int i = 0; i < Q.size(); i++)
        if (!Q[i]) a.F.emplace(i);

    a.serialize();
}
