#include <iostream>
#include <iterator>
#include "simpleregexdfa.h"

using namespace pl;
using namespace pl::regex;

int main()
{
    Regex regex1("abdce");
    Regex regex2("abdce");
    auto result1 = regex1 + regex2;

    auto result2 = regex1 | regex2;

    auto result3 = *regex1;

    regex1 += result2 + result3;

    EpsilonNFA enfa(regex1, "abcdeabcde_or_abcde__rep");
    NFA nfa(enfa);
    DFA dfa(nfa);

}