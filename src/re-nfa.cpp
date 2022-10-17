#include "automata.h"

int main(int argc, char** argv) {
    std::string line;
    std::cin >> line;
    build_nfa(line.c_str()).serialize();
}
