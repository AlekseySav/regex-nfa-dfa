#include "automata.h"

int main() {
    Automata a;
    a.deserialize();

    std::vector<bool> Q(a.size());
    for (int i : a.qfinal)
        Q[i] = 1;
    a.qfinal = {};
    for (int i = 0; i < Q.size(); i++)
        if (!Q[i]) a.qfinal.emplace(i);

    a.serialize();
}
