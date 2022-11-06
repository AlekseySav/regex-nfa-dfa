#include "automata.h"

int main(int argc, char** argv) {
    Automata input;
    input.deserialize();
    remove_epsilons(input);
    input.serialize();
}
