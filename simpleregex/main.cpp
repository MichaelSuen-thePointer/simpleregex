#include "simpleregexparser.h"
#include "simpleregexnfa.h"
using namespace pl;
using namespace pl::regex;

int main()
{

    auto regex1 = "";
    auto regex2 = "a";
    auto regex3 = "abcdefg";
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

    EpsilonNFA nfa = EpsilonNFA::generate(result2, "a");
    nfa.combine_regex(result3, "abcdefg");
    nfa.combine_regex(result5, "astarb");
    nfa.combine_regex(result6, "astara");

    auto result = nfa.match_first("aaaaab");

}