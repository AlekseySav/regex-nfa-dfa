targets += alphabet.h libregex.a libugraphviz.a
targets += trace draw
targets += run cmp-nfa is
targets += re-nfa nfa-1nfa 1nfa-dfa dfa-cdfa cdfa-inversed cdfa-min cdfa-re

ugraphviz-targets = vendor/ugraphviz/graph.o vendor/ugraphviz/registry.o vendor/ugraphviz/subgraph.o
lib-targets = src/lib/complete.o src/lib/inverse.o src/lib/minimize.o src/lib/remove_epsilons.o src/lib/build_nfa.o src/lib/build_dfa.o  src/lib/task15.o

.PHONY: all clean test

all: $(targets)

clean:
	$(RM) $(targets) $(lib-targets) $(ugraphviz-targets)

test: $(targets)
	@tests/run $(test-only)

# default alphabet
alphabet.h: tools/set-alphabet
	$< abcdefghijklmnopqrstuvwxyz

libugraphviz.a: $(ugraphviz-targets)
	ar rcs $@ $^

libregex.a: $(lib-targets)
	ar rcs $@ $^

%: src/%.cpp src/automata.h alphabet.h libregex.a libugraphviz.a
	$(CXX) -std=c++23 -Ivendor -o $@ $< -L. -lugraphviz -lregex
#  -fPIC -fprofile-arcs -ftest-coverage

%.o: %.cpp
	$(CXX) -c -std=c++23 -Ivendor -o $@ $<
