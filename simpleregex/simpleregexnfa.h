#pragma once
#ifndef SIMPLE_REGEX_NFA
#define SIMPLE_REGEX_NFA

#include "simpleregexdecl.h"
#include <vector>
#include <queue>
namespace pl
{
namespace regex
{

using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

class NFA
{
private:
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

    class NFAGenerator: public Regex::IVisitor
    {
    public:
        shared_ptr<Node> start;
        shared_ptr<Node> end;
    private:
        string _name;
        virtual void visit(Empty&) override
        {
            shared_ptr<Node> newNode = std::make_shared<Node>();
            start->strongEdges.push_back(StrongEdge{'\0', newNode});
            end = newNode;
            end->stateName = _name;
        }
        virtual void visit(Char& node) override
        {
            shared_ptr<Node> newNode = std::make_shared<Node>();
            start->strongEdges.push_back(StrongEdge{node.ch(), newNode});
            end = newNode;
            end->stateName = _name;
        }
        virtual void visit(Concat& node) override
        {
            NFAGenerator left, right;
            node.left()->accept(left);
            node.right()->accept(right);
            start = left.start;
            left.end->strongEdges.push_back(StrongEdge{'\0', right.start});
            end = right.end;
            end->stateName = _name;
        }
        virtual void visit(Or& node) override
        {
            NFAGenerator left, right;
            node.left()->accept(left);
            node.right()->accept(right);
            right.start->strongEdges.push_back(StrongEdge{'\0', left.start});
            right.end->strongEdges.push_back(StrongEdge{'\0', left.end});

            start = right.start;
            end = right.end;
            end->stateName = _name;
        }
        virtual void visit(Kleene& node) override
        {
            NFAGenerator expr;
            node.expr()->accept(expr);
            expr.end->weakEdges.push_back(WeakEdge{'\0', expr.start});
            expr.start->weakEdges.push_back(WeakEdge{'\0', expr.end});
            start = expr.start;
            end = expr.end;
            end->stateName = _name;
        }
    public:
        NFAGenerator(const string& name = string())
            : start(new Node(name))
            , end()
            , _name(name)
        {
        }
    };
    shared_ptr<Node> startState;
    vector<shared_ptr<Node>> endStates;
    NFA(shared_ptr<Node> start, shared_ptr<Node> end)
        : startState(start)
        , endStates{end}
    {
    }
public:
    static NFA generate(shared_ptr<Regex> regex, string matchName)
    {
        NFAGenerator generator(matchName);
        regex->accept(generator);
        return NFA(generator.start, generator.end);
    }
    NFA& combine_regex(shared_ptr<Regex> regex, string matchName)
    {
        NFAGenerator generator(matchName);
        regex->accept(generator);
        startState->strongEdges.push_back(StrongEdge{'\0', generator.start});
        endStates.push_back(generator.end);
        return *this;
    }
    vector<string> match(string str)
    {
        using std::queue;
        using state_pair = std::pair<Node*, string::iterator>;
        struct state_pair_compare
        {
            size_t operator()(const state_pair& obj) const
            {
                return std::hash<Node*>()(obj.first);
            }
        };

        vector<string> results;

        queue<state_pair> currentStates;
        
        currentStates.push({startState.get(), str.begin()});

        while (currentStates.size())
        {
            const auto& current = currentStates.front();
            if (current.second == str.end())
            {
                if (current.first->stateName != "")
                {
                    results.push_back(current.first->stateName);
                }
            }
            else
            {
                for (const auto& edge : current.first->strongEdges)
                {
                    if (edge.accept == '\0')
                    {
                        currentStates.insert({edge.next.get(), current.second});
                    }
                    else if (edge.accept == *(current.second))
                    {
                        currentStates.insert({edge.next.get(), current.second + 1});
                    }
                }
                for (const auto& edge : current.first->weakEdges)
                {
                    if (edge.accept == '\0')
                    {
                        currentStates.insert({edge.next.lock().get(), current.second});
                    }
                    else if (edge.accept == *(current.second))
                    {
                        currentStates.insert({edge.next.lock().get(), current.second + 1});
                    }
                }
            }
            currentStates.erase(currentStates.begin());
        }
        return results;
    }
};


}
}

#endif // !SIMPLE_REGEX_VISITOR
