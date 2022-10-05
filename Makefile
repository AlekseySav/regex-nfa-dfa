targets += libugraphviz.a alphabet.h
targets += trace draw
targets += dfa-run nfa-run cmp-nfa is
targets += re-nfa nfa-1nfa 1nfa-dfa dfa-cdfa cdfa-inv cdfa-min cdfa-re

ugraphviz-targets = vendor/ugraphviz/graph.o vendor/ugraphviz/registry.o vendor/ugraphviz/subgraph.o

.PHONY: all clean test

all: $(targets)

clean:
	$(RM) $(targets) $(ugraphviz-targets)

test: $(targets)
	@tests/run $(test-only)

# default alphabet
alphabet.h: tools/set-alphabet
	$< abcdefghijklmnopqrstuvwxyz

libugraphviz.a: $(ugraphviz-targets)
	ar rcs $@ $^

%: src/%.cpp src/automata.h alphabet.h
	$(CXX) -std=c++23 -Ivendor -o $@ $< -L. -lugraphviz

%.o: %.cpp
	$(CXX) -c -std=c++23 -Ivendor -o $@ $<
