#include "automata.h"
#include <algorithm>

static std::vector<int> edges(const Automata& a, int i, int j) {
    std::vector<int> res;
    for (int c = 0; c < W; c++) {
        auto& v = a.nodes[i][c];
        if (std::find(v.begin(), v.end(), j) != v.end())
            res.push_back(c);
    }
    return res;
}

char color_hint(const Automata& a, int i, int j) {
    char c = 0;
    if (a.q0 == i) c |= 1;
    if (a.F.contains(j)) c |= 2;
    return c ? c : 7;
}

void print_char(char c, char color_hint) {
    if (c == '\e') {
        color_hint = 6;
        c = 'e';
    }
    printf("\e[3%cm%c\e[0m ", color_hint + '0', c);
}

static void print(const Automata& a) {
    printf("  ");
    for (int i = 0; i < a.size(); i++)
        print_char(i + '0', color_hint(a, -1, i));
    putchar('\n');

    for (int i = 0; i < a.size(); i++) {
        std::vector<std::vector<int>> chains;
        print_char(i + '0', color_hint(a, i, -1));
        for (int j = 0; j < a.size(); j++) {
            auto v = edges(a, i, j);
            if (v.size() > 1) {
                print_char('*', color_hint(a, i, j));
                chains.push_back(v);
                continue;
            }
            char c = v.size() ? alpha(v[0]) : '-';
            print_char(c, color_hint(a, i, j));
        }
        for (auto& i : chains) {
            putchar('[');
            putchar(' ');
            for (int c : i)
                print_char(alpha(c), 7);
            putchar(']');
        }
        putchar('\n');
    }
    putchar('\n');
}

int main() {
    Automata a;
    a.deserialize();
    print(a);
}
