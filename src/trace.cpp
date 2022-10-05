#include "automata.h"
#include <algorithm>

char color_hint(const Automata& a, int i, int j) {
    char c = 0;
    if (a.q0 == i) c |= 1;
    if (a.qfinal.contains(j)) c |= 2;
    return c ? c : 7;
}

void print_char(char c, char color_hint) {
    if (c == '\e') {
        color_hint = 6;
        c = 'e';
    }
    std::cout << "\e[3" << (char)(color_hint + '0') << "m" << c << "\e[0m ";
}

static void print(const Automata& a) {
    std::cout << "  ";
    for (int i = 0; i < a.size(); i++)
        print_char(i + '0', color_hint(a, -1, i));
    std::cout << '\n';

    for (int i = 0; i < a.size(); i++) {
        std::vector<std::vector<int>> chains;
        print_char(i + '0', color_hint(a, i, -1));
        for (int j = 0; j < a.size(); j++) {
            auto v = a.get_edges(i, j);
            if (v.size() > 1) {
                print_char('*', color_hint(a, i, j));
                chains.push_back(v);
                continue;
            }
            char c = v.size() ? alpha(v[0]) : '-';
            print_char(c, color_hint(a, i, j));
        }
        for (auto& i : chains) {
            std::cout << '[';
            std::cout << ' ';
            for (int c : i)
                print_char(alpha(c), 7);
            std::cout << ']';
        }
        std::cout << '\n';
    }
    std::cout << '\n';
}

int main() {
    Automata a;
    a.deserialize();
    print(a);
}
