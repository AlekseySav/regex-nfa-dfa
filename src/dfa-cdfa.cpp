#include "automata.h"

int main() {
    Automata input;
    input.deserialize();
    complete_dfa(input);
    input.serialize();
}
