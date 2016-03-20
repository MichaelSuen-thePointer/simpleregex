#include "simpleregexfsm.h"
namespace pl
{
namespace regex
{

tuple<vector<array<int, 256>>,
    vector<StateInfo>,
    int>
    FSM::generate(const DFA & dfa)
{
    vector<array<int, 256>> _stateMachine(dfa.node_pool().size() + 1, array<int, 256>());
    vector<StateInfo> _endStates;
    map<const Node*, int> nodeSet2State;
    int maxEndStateLabel = -1;
    nodeSet2State[dfa.start()] = 0;

    int _invalidState = dfa.node_pool().size();
    _stateMachine.reserve(_invalidState + 1);
    for (auto& node : dfa.node_pool())
    {
        if (nodeSet2State.find(node.get()) == nodeSet2State.end())
        {
            nodeSet2State[node.get()] = nodeSet2State.size();
        }
    }
    for (auto& node : dfa.node_pool())
    {
        int stateLabel = nodeSet2State[node.get()];
        std::fill(_stateMachine[stateLabel].begin(), _stateMachine[stateLabel].end(), _invalidState);
        for (auto& edge : node->edges)
        {
            _stateMachine[stateLabel][edge.accept] = nodeSet2State[edge.next];
        }
        if (node->stateInfo.label != Node::MIDDLE_STATE)
        {
            if (maxEndStateLabel < nodeSet2State[node.get()])
            {
                maxEndStateLabel = nodeSet2State[node.get()];
            }
        }
    }
    _endStates.clear();
    _endStates.assign(maxEndStateLabel + 1, StateInfo());
    for (auto& node : dfa.node_pool())
    {
        if (node->stateInfo.label != Node::MIDDLE_STATE)
        {
            _endStates[nodeSet2State[node.get()]] = node->stateInfo;
        }
    }

    return std::make_tuple(std::move(_stateMachine), std::move(_endStates), _invalidState);
}

}
}