#pragma once
#ifndef SIMPLE_REGEX_DFA
#define SIMPLE_REGEX_DFA

#include <vector>
#include <memory>
#include <algorithm>
#include "simpleregexnfa.h"

namespace pl
{
namespace regex
{

using std::vector;
using std::unique_ptr;
using std::pair;

class DFA
{
protected:
    vector<unique_ptr<Node>> _pool;
    Node* _start;

    static pair<vector<unique_ptr<Node>>, Node*> generate(const NFA& nfa);
public:
    DFA(const NFA& nfa)
    {
        auto dfa = DFA::generate(nfa);
        _pool = std::move(dfa.first);
        _start = dfa.second;
    }

    const vector<unique_ptr<Node>>& node_pool() const { return _pool; }
    
    Node* start() { return _start; }
    const Node* start() const { return _start; }
};


}
}
#endif // !SIMPLE_REGEX_DFA
