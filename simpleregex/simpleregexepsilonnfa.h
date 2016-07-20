#pragma once
#ifndef SIMPLE_REGEX_EPSILON_NFA
#define SIMPLE_REGEX_EPSILON_NFA

#include "simpleregex.h"
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
        StateInfo _info;
        virtual void visit(Empty&) override;
        virtual void visit(Char&) override;
        virtual void visit(Concat&) override;
        virtual void visit(Alternative&) override;
        virtual void visit(Kleene&) override;
        virtual void visit(CharRange &) override;

        void adopt_pool(vector<unique_ptr<Node>>& rhs);
    public:
        NFAGenerator(const StateInfo& info = StateInfo())
            : _pool()
            , start()
            , end()
            , _info(info)
        {
            _pool.push_back(std::make_unique<Node>());
            start = _pool.front().get();
        }

        vector<unique_ptr<Node>>&& move_pool()
        {
            return std::move(_pool);
        }
    };

    vector<unique_ptr<Node>> _pool;
    Node* _startState;
    vector<Node*> _endStates;

    EpsilonNFA(NFAGenerator& generator)
        : _pool(generator.move_pool())
        , _startState(generator.start)
        , _endStates{generator.end}
    {
    }
public:
    EpsilonNFA()
        : _pool()
        , _startState()
        , _endStates()
    {
    }
    EpsilonNFA(Regex& regex, const StateInfo& matchName)
        : _pool()
        , _startState()
        , _endStates()
    {
        NFAGenerator generator(matchName);
        regex.regex()->accept(generator);
        _pool = generator.move_pool();
        _startState = generator.start;
        _endStates.push_back(generator.end);
    }

    static EpsilonNFA generate(Regex& regex, const StateInfo& matchName);
    static EpsilonNFA generate(const shared_ptr<IRegex>& regex, const StateInfo& matchName);

    Node* start_state() const { return _startState; }
    const vector<Node*>& end_states() const { return _endStates; }

    EpsilonNFA& combine_regex(IRegex* regex, const StateInfo& matchName);
    EpsilonNFA& combine_regex(const wstring& regex, const StateInfo& matchName);
    EpsilonNFA& combine_regex(Regex& regex, const StateInfo& matchName);
};


}
}

#endif // !SIMPLE_REGEX_VISITOR
