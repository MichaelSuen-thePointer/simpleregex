#pragma once
#ifndef SIMPLE_REGEX_FSM
#define SIMPLE_REGEX_FSM

#include <array>
#include <deque>
#include "simpleregexdfa.h"

namespace pl
{
namespace regex
{

using std::vector;
using std::array;
using std::priority_queue;
using std::string;

class FSM
{
protected:
    vector<array<int, 255>> _stateMachine;

    static vector<array<int, 255>> generate(const DFA& dfa)
    {
        vector<array<int, 255>> _stateMachine;
        map<const Node*, int> nodeSet;
        nodeSet[dfa.start()] = 0;

        int invalidState = dfa.node_pool().size();
        _stateMachine.reserve(invalidState + 1);
        for (auto& node : dfa.node_pool())
        {
            if (nodeSet.find(node.get()) == nodeSet.end())
            {
                nodeSet[node.get()] = nodeSet.size();
            }
        }
        for(auto& node : dfa.node_pool())
        {
            int stateLabel = nodeSet[node.get()];
            std::fill(_stateMachine[stateLabel].begin(), _stateMachine[stateLabel].end(), invalidState);
            for (auto& edge : node->edges)
            {
                _stateMachine[stateLabel][edge.accept] = nodeSet[edge.next];
            }
        }

        return std::move(_stateMachine);
    }
public:
    FSM(const DFA& dfa)
        : _stateMachine(generate(dfa))
    {
    }
};

}
}
#endif // !SIMPLE_REGEX_FSM
