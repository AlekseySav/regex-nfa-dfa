#include "automata.h"

int main() {
    Automata input;
    input.deserialize();
    inverse_cdfa(input);
    input.serialize();
}
