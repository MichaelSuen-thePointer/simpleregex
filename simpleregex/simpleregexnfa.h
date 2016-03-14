#pragma once
#ifndef SIMPLE_REGEX_NFA
#define SIMPLE_REGEX_NFA

#include "simpleregexdecl.h"
#include "distinct_queue.h"
#include <vector>
namespace pl
{
namespace regex
{

using std::string;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;

class EpsilonNFA
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
        virtual void visit(Empty&) override;
        virtual void visit(Char&) override;
        virtual void visit(Concat&) override;
        virtual void visit(Or&) override;
        virtual void visit(Kleene&) override;
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
    EpsilonNFA(shared_ptr<Node> start, shared_ptr<Node> end)
        : startState(start)
        , endStates{end}
    {
    }
public:
    static EpsilonNFA generate(shared_ptr<Regex> regex, string matchName);
    EpsilonNFA& combine_regex(shared_ptr<Regex> regex, string matchName);
    /*
    string match_first(string str)
    {
        using container::distinct_queue;
        using state_pair = std::pair<Node*, string::iterator>;

        struct state_pair_equal
        {
            bool operator()(const state_pair& lhs, const state_pair& rhs) const
            {
                return lhs.first == rhs.first;
            }
        };
        distinct_queue<state_pair, state_pair_equal> currentStates;

        currentStates.push({startState.get(), str.begin()});

        while (currentStates.size())
        {
            const auto& current = currentStates.front();
            if (current.second == str.end())
            {
                if (current.first->stateName != "")
                {
                    return current.first->stateName;
                }
            }
            else
            {
                for (const auto& edge : current.first->strongEdges)
                {
                    if (edge.accept == '\0')
                    {
                        currentStates.push({edge.next.get(), current.second});
                    }
                    else if (edge.accept == *(current.second))
                    {
                        currentStates.push({edge.next.get(), current.second + 1});
                    }
                }
                for (const auto& edge : current.first->weakEdges)
                {
                    if (edge.accept == '\0')
                    {
                        currentStates.push({edge.next.lock().get(), current.second});
                    }
                    else if (edge.accept == *(current.second))
                    {
                        currentStates.push({edge.next.lock().get(), current.second + 1});
                    }
                }
            }
            currentStates.pop();
        }
        return "";
    }
    */
};


}
}

#endif // !SIMPLE_REGEX_VISITOR
