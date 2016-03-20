#pragma once
#ifndef SIMPLE_REGEX_AUTOMATION_NODE
#define SIMPLE_REGEX_AUTOMATION_NODE

#include <memory>
#include <vector>
#include <string>

namespace pl
{
namespace regex
{

using std::shared_ptr;
using std::weak_ptr;
using std::vector;
using std::string;

struct StateInfo
{
    int label;
    string name;
    StateInfo(int _label = -1, string _name = "")
        : label(_label)
        , name(_name)
    {
    }
};

struct Node;
struct Edge
{
    char accept;
    Node* next;
};

struct Node
{
    vector<Edge> edges;
    StateInfo stateInfo;
    enum
    {
        MIDDLE_STATE = -1
    };
    Node(int label = MIDDLE_STATE, const string& name = string())
        : stateInfo(label, name)
    {
    }
    Node(const StateInfo& info)
        : stateInfo(info)
    {
    }
};



}
}

#endif // !SIMPLE_REGEX_AUTOMATION_NODE