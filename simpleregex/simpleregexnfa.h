#pragma once

#ifndef SIMPLE_REGEX_NFA
#define SIMPLE_REGEX_NFA

#include <memory>
#include <set>
#include <map>
#include <functional>
#include <queue>
#include "simpleregexepsilonnfa.h"

namespace pl
{
namespace regex
{

using std::set;
using std::function;
using std::map;
using std::queue;

class NFA
{
protected:
    shared_ptr<Node> startState;

    static void BFS_search(const EpsilonNFA& enfa, const function<void(Node*)> apply);

    static set<Node*> find_valid_states(const EpsilonNFA& enfa);

    static set<const Node*> find_epsilon_closure(const Node* node);


public:
    static NFA generate(const EpsilonNFA& enfa);

    vector<string> match_all(const string& text);
};


}
}
#endif // !SIMPLE_REGEX_NFA
