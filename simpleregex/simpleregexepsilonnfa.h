#pragma once
#ifndef SIMPLE_REGEX_EPSILON_NFA
#define SIMPLE_REGEX_EPSILON_NFA

#include "simpleregexparser.h"
#include "simpleregexdecl.h"
#include "distinct_queue.h"
#include "simpleregexautomationnode.h"
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

    class NFAGenerator: public IRegex::IVisitor
    {
    public:
        vector<unique_ptr<Node>> _pool;
        Node* start;
        Node* end;
    private:
        string _name;
        virtual void visit(Empty&) override;
        virtual void visit(Char&) override;
        virtual void visit(Concat&) override;
        virtual void visit(Or&) override;
        virtual void visit(Kleene&) override;

        void adopt_pool(vector<unique_ptr<Node>>& rhs);
    public:
        NFAGenerator(const string& name = string())
            : _pool()
            , start()
            , end()
            , _name(name)
        {
            _pool.push_back(std::make_unique<Node>());
            start = _pool.front().get();
        }
    };
    vector<unique_ptr<Node>> _pool;
    Node* _startState;
    vector<Node*> _endStates;
    EpsilonNFA(vector<unique_ptr<Node>>&& pool, Node* start, Node* end)
        : _pool(std::move(pool))
        , _startState(start)
        , _endStates{end}
    {
    }
public:
    Node* start_state() const { return _startState; }
    const vector<Node*>& end_states() const { return _endStates; }

    static EpsilonNFA generate(const shared_ptr<IRegex>& regex, const string& matchName);
    EpsilonNFA& combine_regex(const shared_ptr<IRegex>& regex, const string& matchName);
    EpsilonNFA& combine_regex(const string& regex, const string& matchName);
};


}
}

#endif // !SIMPLE_REGEX_VISITOR
