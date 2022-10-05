#include "automata.h"
#include <sstream>
#include <algorithm>

void differ(const char* msg) {
    std::cout << "differ!! " << msg << "\n";
    exit(1);
}

bool compare(Automata& a, Automata& b, const std::vector<int>& perm) {
    if (a.q0 != perm[b.q0]) return false;
    for (int i : b.qfinal)
        if (!a.qfinal.contains(perm[i]))
            return false;

    for (int from = 0; from < b.size(); from++) {
        for (auto&[c, set] : b.nodes[from]) {
            if (set.size() != a.nodes[perm[from]][c].size())
                return false;
            for (int to : set)
                if (!a.nodes[perm[from]][c].contains(perm[to]))
                    return false;
        }
    }
    return true;
}

int main(int argc, char** argv) {
    std::stringstream ss(argv[1]);
    Automata a, b;
    a.deserialize();
    b.deserialize(ss);

    if (a.size() != b.size())
        differ("nodes count differ");
    if (a.qfinal.size() != b.qfinal.size())
        differ("qfinal count differ");

    std::vector<int> perm(a.size());
    for (int i = 0; i < a.size(); i++)
        perm[i] = i;

    do {
        if (compare(a, b, perm))
            return 0;
    } while (std::next_permutation(perm.begin(), perm.end()));
    differ("edges differ");
}
