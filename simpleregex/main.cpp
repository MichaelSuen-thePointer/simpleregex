#include <vld.h>
#include <iostream>
#include <iterator>
#include "simpleregexfsm.h"

using namespace pl;
using namespace pl::regex;

int main()
{
    Regex regex1("(a|b|c|d|e|f|g|h|i|j|k|l|m|n|o|p|q|r|s|t|u|v|w|x|y|z)*");
    Regex regex2("keyword");
    EpsilonNFA enfa(regex1, "id");
    enfa.combine_regex(regex2, "kw");
    NFA nfa(enfa);
    DFA dfa(nfa);
    FSM fsm(dfa);

    fsm.debug_print(std::cout);
    std::cin.get();
}