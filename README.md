# Regex, NFA, DFA, ...

## structure

all subtasks are presented as separate programs

## programs

#### required by task:

- ```re-nfa```               &mdash; parse regex into NFA
- ```nfa-1nfa```             &mdash; remove all epsilon-edges from NFA
- ```1nfa-dfa```             &mdash; convert NFA with no epsilon-edges into DFA
- ```dfa-cdfa```             &mdash; convert DFA to CDFA
- ```cdfa-inversed```        &mdash; inverse CDFA
- ```cdfa-min```             &mdash; convert CDFA to minimal CDFA
- ```cdfa-re```              &mdash; convert CDFA to regex

#### ones that improve debugging and quality of life:

- ```is <type>...```        &mdash; check if NFA is type=[nfa,1nfa,dfa,cdfa] 
- ```trace```               &mdash; nicely print automata
- ```draw```                &mdash; convert automata to graphviz
- ```cmp-nfa <nfa>```       &mdash; compare NFA from stdin with NFA from argv[1]
- ```run [-f] <src>```      &mdash; run NFA on each line in file or string, depending on ```-f``` flag 

### scripts:

- ```tools/cmp <line>```    &mdash; compare line from stdin and line from argv[1]
- ```tools/input```         &mdash; read NFA
- ```tools/isempty```       &mdash; assert stdin is not empty

### note

all programs can be chained using pipes

for example:

```$ echo $regex | re-nfa | trace```
    will build NFA from ```$regex``` and print it

```$ echo $regex | re-nfa | nfa-1nfa | 1nfa-dfa | dfa-cdfa | run -f 1.txt```
    will build CDFA base on ```$regex``` and run it for each line in ```1.txt```

### note 2: shortcut

- ```shortcut``` &mdash; implicitly chains intermediate programs

usage:

```shortcut [-i] [-p] [-d] input-format output-format [input] [>output-file]```

- `-i` &mdash; run `tools/input` to format input data
- `-p` &mdash; run `./trace` for output data
- `-d` &mdash; run `./draw | dot -Tpng` for output data
- `i/o format` &mdash; `re` `nfa` `1nfa` `dfa` `cdfa` `min` `re`
- `input` &mdash; if specified, used as input data, overwise stdin is used

## customizations

it's possible to change alphabet with
```$ tools/set-alphabet <new-alphabet>```
    (epsilon will be implicitly added to the alphabet)

by default, make runs
```$ tools/set-alphabet "abcdefghijklmnopqrstuvwxyz"```

after updating alphabet, it's required to re-build project

to reset default alphabet run
```$ make clean all```

## tests
```make test [test-only="name..."]``` &mdash; run tests, specified by test-only, or all tests if test-only is undefined

### currently present tests:
- ```run```
- ```cmp-nfa```
- ```nfa-1nfa```
- ```1nfa-dfa```
- ```dfa-cdfa```
- ```cdfa-inv```
- ```re-nfa```
- ```cdfa-min```
- ```cdfa-re```
