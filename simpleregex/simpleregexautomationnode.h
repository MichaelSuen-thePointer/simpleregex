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
struct Edge
{
    char accept;
    Node* next;
};
struct Node
{
    vector<Edge> edges;
    string stateName;
    Node(const string& name = string())
        : stateName(name)
    {
    }
};

}
}

#endif // !SIMPLE_REGEX_AUTOMATION_NODE