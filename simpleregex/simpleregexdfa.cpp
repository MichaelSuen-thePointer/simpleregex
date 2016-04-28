#include "simpleregexdfa.h"

namespace pl
{
namespace regex
{

pair<vector<unique_ptr<Node>>, Node*> DFA::generate(const NFA & nfa)
{
    vector<unique_ptr<Node>> newNodePool;
    map<set<const Node*>, Node*> nodeSet2New;
    queue<set<const Node*>> nodeQueue;

    Node* start;
    //nfa.debug_print(std::cout);
    nodeQueue.push({nfa.start()});
    newNodePool.push_back(std::make_unique<Node>(nfa.start()->stateInfo));
    nodeSet2New[nodeQueue.front()] = newNodePool.back().get();
    start = newNodePool.back().get();

    while (nodeQueue.size())
    {
        auto& nodeSet = nodeQueue.front();
        map<unsigned char, set<const Node*>> newEdges;
        for (auto& node : nodeSet)
        {
            for (auto& edge : node->edges)
            {
                newEdges[edge.accept].insert(edge.next);
            }
        }
        /*
        std::for_each(nodeSet.begin(), nodeSet.end(), [](auto& node) {
        std::for_each(node->edges.begin(), node->edges.end(), [](auto& edge) {
        newEdges[edge.accept].insert(edge.next);
        });
        });*/
        for (auto edge : newEdges)
        {
            auto& nodeTo = edge.second;
            if (nodeSet2New.find(nodeTo) == nodeSet2New.end())
            {
                StateInfo stateName;
                std::for_each(nodeTo.begin(), nodeTo.end(), [&stateName](auto elm) {
                    if ((stateName.label == Node::MIDDLE_STATE &&
                         elm->stateInfo.label != Node::MIDDLE_STATE) ||
                        (elm->stateInfo.label != Node::MIDDLE_STATE &&
                         stateName.label > elm->stateInfo.label))
                    {
                        stateName = elm->stateInfo;
                    }
                });
                newNodePool.push_back(std::make_unique<Node>(stateName));
                nodeSet2New[nodeTo] = newNodePool.back().get();
                nodeQueue.push(nodeTo);
            }
            nodeSet2New[nodeSet]->edges.push_back(Edge{edge.first, nodeSet2New[nodeTo]});
        }
        nodeQueue.pop();
    }

    return std::make_pair(std::move(newNodePool), start);
}

}
}