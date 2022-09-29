targets = trace dfa-run re-nfa nfa-1nfa 1nfa-dfa dfa-cdfa cdfa-inv alphabet.h

.PHONY: all clean test

all: $(targets)
	@echo done

clean:
	$(RM) $(targets)

test: $(targets)
	@tests/run

# default alphabet
alphabet.h: tools/set-alphabet
	$< abcdefghijklmnopqrstuvwxyz

%: src/%.cpp src/automata.h alphabet.h
	$(CXX) -std=c++20 -o $@ $<
