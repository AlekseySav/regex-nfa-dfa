#include "automata.h"

int main() {
    Automata input;
    input.deserialize();
    build_dfa(input);
    input.serialize();
}
