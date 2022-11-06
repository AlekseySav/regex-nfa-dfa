#include "automata.h"
#include <algorithm>

char color_hint(const Automata& automata, int line, int column) {
    char symbol = 0;
    if (automata.entry_state == line) symbol |= 1;
    if (automata.final_states.contains(column)) symbol |= 2;
    return symbol ? symbol : 7;
}

void print_char(char symbol, char color_hint) {
    if (symbol == '\e') {
        color_hint = 6;
        symbol = 'e';
    }
    std::cout << "\e[3" << (char)(color_hint + '0') << "m" << symbol << "\e[0m ";
}

void print_line(const Automata& automata, int line) {
    std::vector<std::vector<int>> chains;
    print_char(line + '0', color_hint(automata, line, -1));
    for (int column = 0; column < automata.size(); column++) {
        auto to = automata.get_edges(line, column);
        if (to.size() > 1) {
            print_char('*', color_hint(automata, line, column));
            chains.push_back(to);
            continue;
        }
        char symbol = to.size() ? alpha(to[0]) : '-';
        print_char(symbol, color_hint(automata, line, column));
    }
    for (auto& chain : chains) {
        std::cout << '[';
        std::cout << ' ';
        for (int symbol : chain)
            print_char(alpha(symbol), 7);
        std::cout << ']';
    }
    std::cout << '\n';
}

void print(const Automata& automata) {
    std::cout << "  ";
    for (int line = 0; line < automata.size(); line++)
        print_char(line + '0', color_hint(automata, -1, line));
    std::cout << '\n';

    for (int line = 0; line < automata.size(); line++) {
        print_line(automata, line);
    }
    std::cout << '\n';
}

int main() {
    Automata input;
    input.deserialize();
    print(input);
}
