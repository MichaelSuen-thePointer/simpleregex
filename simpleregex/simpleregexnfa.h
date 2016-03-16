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
    vector<unique_ptr<Node>> _pool;
    Node* startState;

    static void BFS_search(const EpsilonNFA& enfa, const function<void(Node*)> apply);

    static set<Node*> find_valid_states(const EpsilonNFA& enfa);

    static set<const Node*> find_epsilon_closure(const Node* node);


public:
    static NFA generate(const EpsilonNFA& enfa);

    vector<string> match_all(const string& text);

    void debug_print(std::ostream& os)
    {
        map<Node*, bool> visited;
        queue<Node*> toVisit;
        toVisit.push(startState);
        while (toVisit.size())
        {
            Node* current = toVisit.front();
            
            os << current << "\t";
            os << (current->stateName.size() ? current->stateName : "\t" ) << "\n";

            visited[current] = true;
            for (const auto& edge : current->edges)
            {
                os << "\t" << edge.accept << " S->" << edge.next<< "\n";

                if (visited.find(edge.next) == visited.end())
                {
                    toVisit.push(edge.next);
                }
            }
            toVisit.pop();
        }
    }
};


}
}
#endif // !SIMPLE_REGEX_NFA
