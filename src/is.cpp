#include "automata.h"

Automata a;

typedef bool (*check)();

void do_assert(const char* name, bool cond) {
    if (!cond) {
        std::cerr << "- is this " << name << "? - no, it's not!\n";
        exit(1);
    }
}

bool is_nfa() { return true; }

bool is_1nfa() {
    for (auto& n : a.nodes)
        if (n[0].size())
            return false;
    return true;
}

bool is_dfa() {
    if (!is_1nfa()) return false;
    for (auto& n : a.nodes)
        for (auto&[c, to] : n)
            if (to.size() > 1)
                return false;
    return true;
}

bool is_cdfa() {
    if (!is_dfa()) return false;
    for (auto& n : a.nodes)
        for (int c = 1; c < W; c++)
            if (n[c].size() != 1)
                return false;
    return true;
}

int main(int argc, char** argv) {
    a.deserialize();

    std::unordered_map<std::string, check> checks = {
        { "nfa", is_nfa },
        { "1nfa", is_1nfa },
        { "dfa", is_dfa },
        { "cdfa", is_cdfa },
    };

    for (int i = 1; i < argc; i++)
        do_assert(argv[i], checks[argv[i]]());
}
