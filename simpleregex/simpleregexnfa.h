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
    vector<shared_ptr<Node>> endStates;

    static void BFS_search(const EpsilonNFA& enfa, const function<void(Node*)> apply)
    {
        map<Node*, bool> visited;
        queue<Node*> toVisit;
        toVisit.push(enfa.start_state());
        while (toVisit.size())
        {
            Node* current = toVisit.front();
            apply(current);
            visited[current] = true;
            for (const auto& edge : current->strongEdges)
            {
                if (visited.find(edge.next.get()) == visited.end())
                {
                    toVisit.push(edge.next.get());
                }
            }
            for (const auto& edge : current->weakEdges)
            {
                if (visited.find(edge.next.lock().get()) == visited.end())
                {
                    toVisit.push(edge.next.lock().get());
                }
            }
            toVisit.pop();
        }
    }

    static set<Node*> find_valid_states(const EpsilonNFA& enfa)
    {
        set<Node*> validStates;
        auto apply = [&validStates](Node* node) {
            for (const auto& edge : node->strongEdges)
            {
                if (edge.accept != '\0')
                {
                    validStates.insert(edge.next.get());
                }
            }
            for (const auto& edge : node->weakEdges)
            {
                if (edge.accept != '\0')
                {
                    validStates.insert(edge.next.lock().get());
                }
            }
        };
        BFS_search(enfa, apply);
        validStates.insert(enfa.start_state());
        return validStates;
    }

    static set<Node*> find_epsilon_closure(const Node* node)
    {

    }
public:
    static NFA generate(const EpsilonNFA& enfa)
    {
        set<Node*> validStates = find_valid_states(enfa);
        
    }

};


}
}
#endif // !SIMPLE_REGEX_NFA
