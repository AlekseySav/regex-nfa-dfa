#include "automata.h"

int main() {
    Automata input;
    input.deserialize();

    std::vector<bool> Q(input.size());
    for (int i : input.qfinal)
        Q[i] = 1;
    input.qfinal = {};
    for (int i = 0; i < Q.size(); i++)
        if (!Q[i]) input.qfinal.emplace(i);

    input.serialize();
}
