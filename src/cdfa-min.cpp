#include "automata.h"

int main() {
    Automata input;
    input.deserialize();
    minimize_cdfa(input);
    input.serialize();
}
