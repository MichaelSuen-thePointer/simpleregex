#include <vld.h>
#include <iostream>
#include <iterator>
#include "simpleregexparser.h"
#include "simpleregexepsilonnfa.h"
#include "simpleregexnfa.h"
using namespace pl;
using namespace pl::regex;

int main()
{

    auto regex1 = "";
    auto regex2 = "a";
    auto regex3 = "aaaaaaab";
    auto regex4 = "a|b|c|d|e";
    auto regex5 = "a*b";
    auto regex6 = "a*a";
    auto regex7 = "(abc)def";
    auto regex8 = "(abc)*def";
    auto regex9 = "(a|b*)|(d*e)";


    RegexParser parser;

    auto result1 = parser.parse(regex1);
    auto result2 = parser.parse(regex2);
    auto result3 = parser.parse(regex3);
    auto result4 = parser.parse(regex4);
    auto result5 = parser.parse(regex5);
    auto result6 = parser.parse(regex6);
    auto result7 = parser.parse(regex7);
    auto result8 = parser.parse(regex8);
    auto result9 = parser.parse(regex9);

    EpsilonNFA enfa = EpsilonNFA::generate(result2, "a");
    enfa.combine_regex(result3, "7ab");
    enfa.combine_regex(result5, "astarb");
    enfa.combine_regex(result6, "astara");

    NFA nfa = NFA::generate(enfa);
    
    auto results = nfa.match_all("aaaaaaab");
    
    nfa.debug_print(std::cout);

    std::copy(results.begin(), results.end(), std::ostream_iterator<string>(std::cout, "\n"));
}