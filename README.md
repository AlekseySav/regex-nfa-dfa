# Regex, NFA, DFA, ...

## structure

all subtasks are presented as separate programs

## programs

#### required by task:

- ```in-nfa```               &mdash; read NFA
- ```re-nfa```               &mdash; parse regex into NFA
- ```nfa-1nfa```             &mdash; remove all epsilon-edges from NFA
- ```1nfa-dfa```             &mdash; convert NFA with no epsilon-edges into DFA
- ```dfa-cdfa```             &mdash; convert DFA to CDFA
- ```cdfa-inv```             &mdash; inverse CDFA
- ```cdfa-min```             &mdash; convert CDFA to minimal CDFA (not implemented yet)
- ```cdfa-re```              &mdash; convert CDFA to regex

#### ones that improve debugging and quality of life:

- ```trace```               &mdash; nicely print automata
- ```dfa-run <file>```      &mdash; based on DFA/CDFA performs search through each line in file

### note

all programs can be chained using pipes

for example:

```$ echo $regex | re-nfa | trace```
    will build NFA from ```$regex``` and print it

```$ echo $regex | re-nfa | nfa-1nfa | 1nfa-dfa | dfa-cdfa | dfa-run 1.txt```
    will build CDFA base on ```$regex``` and run it for each line in ```1.txt```

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

in progress....
