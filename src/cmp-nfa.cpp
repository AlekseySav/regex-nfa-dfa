#include "automata.h"
#include <sstream>
#include <algorithm>

void differ(const char* msg) {
    std::cout << "differ!! " << msg << "\n";
    exit(1);
}

bool compare_automates(Automata& first_automata, Automata& second_automata, const std::vector<int>& perm) {
    if (first_automata.entry_state != perm[second_automata.entry_state]) return false;
    for (int final : second_automata.final_states) {
        if (!first_automata.final_states.contains(perm[final])) {
            return false;
        }
    }

    for (int from = 0; from < second_automata.size(); from++) {
        for (auto&[symbol, set] : second_automata.nodes[from]) {
            if (set.size() != first_automata.nodes[perm[from]][symbol].size()) {
                return false;
            }
            for (int to : set) {
                if (!first_automata.nodes[perm[from]][symbol].contains(perm[to])) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main(int argc, char** argv) {
    std::stringstream ss(argv[1]);
    Automata first_automata, second_automata;
    first_automata.deserialize();
    second_automata.deserialize(ss);

    if (first_automata.size() != second_automata.size()) {
        differ("nodes count differ");
    }
    if (first_automata.final_states.size() != second_automata.final_states.size()) {
        differ("final_states count differ");
    }

    std::vector<int> perm(first_automata.size());
    for (int i = 0; i < first_automata.size(); i++)
        perm[i] = i;

    do {
        if (compare_automates(first_automata, second_automata, perm))
            return 0;
    } while (std::next_permutation(perm.begin(), perm.end()));
    differ("edges differ");
}
