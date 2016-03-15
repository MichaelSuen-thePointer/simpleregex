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

struct Node;
struct StrongEdge;
struct StrongEdge
{
    char accept;
    shared_ptr<Node> next;
};
struct WeakEdge
{
    char accept;
    weak_ptr<Node> next;
};
struct Node
{
    vector<StrongEdge> strongEdges;
    vector<WeakEdge> weakEdges;
    string stateName;
    Node(const string& name = string())
        : stateName(name)
    {
    }
};


}
}

#endif // !SIMPLE_REGEX_AUTOMATION_NODE