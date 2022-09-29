targets = trace dfa-run re-nfa nfa-1nfa 1nfa-dfa dfa-cdfa cdfa-inv cdfa-re alphabet.h

.PHONY: all clean test

all: $(targets)

clean:
	$(RM) $(targets)

test: $(targets)
	@tests/run

# default alphabet
alphabet.h: tools/set-alphabet
	$< abcdefghijklmnopqrstuvwxyz

%: src/%.cpp src/automata.h alphabet.h
	$(CXX) -std=c++23 -o $@ $<
