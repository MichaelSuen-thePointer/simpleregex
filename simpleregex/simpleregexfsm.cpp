#include "simpleregexfsm.h"
namespace pl
{
namespace regex
{

tuple<vector<array<int, 256>>,
      map<int, StateInfo>,
      int>
FSM::generate(const DFA & dfa)
{
    vector<array<int, 256>> _stateMachine(dfa.node_pool().size() + 1, array<int, 256>());
    map<int, StateInfo> _endState;
    map<const Node*, int> nodeSet;
    nodeSet[dfa.start()] = 0;

    int _invalidState = dfa.node_pool().size();
    _stateMachine.reserve(_invalidState + 1);
    for (auto& node : dfa.node_pool())
    {
        if (nodeSet.find(node.get()) == nodeSet.end())
        {
            nodeSet[node.get()] = nodeSet.size();
        }
    }
    for (auto& node : dfa.node_pool())
    {
        int stateLabel = nodeSet[node.get()];
        std::fill(_stateMachine[stateLabel].begin(), _stateMachine[stateLabel].end(), _invalidState);
        for (auto& edge : node->edges)
        {
            _stateMachine[stateLabel][edge.accept] = nodeSet[edge.next];
        }
        if (node->stateInfo.label != Node::NOT_END_STATE)
        {
            _endState[nodeSet[node.get()]] = node->stateInfo;
        }
    }

    return std::make_tuple(std::move(_stateMachine), std::move(_endState), _invalidState);
}

}
}