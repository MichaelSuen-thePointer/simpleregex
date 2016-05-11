#include "simpleregexepsilonnfa.h"
namespace pl
{
namespace regex
{

void EpsilonNFA::NFAGenerator::visit(Empty&)
{
    end = start;
    end->stateInfo = _info;
}

void EpsilonNFA::NFAGenerator::visit(Char& node)
{
    _pool.push_back(std::make_unique<Node>());
    auto newNode = _pool.back().get();
    start->edges.push_back(Edge{node.ch(), newNode});
    end = newNode;
    end->stateInfo = _info;
}

void EpsilonNFA::NFAGenerator::visit(Concat& node)
{
    NFAGenerator left, right;
    node.left()->accept(left);
    node.right()->accept(right);

    using iter_type = decltype(_pool)::iterator;

    adopt_pool(left._pool);
    adopt_pool(right._pool);

    start = left.start;
    left.end->edges.push_back(Edge{'\0', right.start});
    end = right.end;
    end->stateInfo = _info;
}

void EpsilonNFA::NFAGenerator::visit(Alternative& node)
{
    NFAGenerator left, right;
    node.left()->accept(left);
    node.right()->accept(right);

    adopt_pool(left._pool);
    adopt_pool(right._pool);

    right.start->edges.push_back(Edge{'\0', left.start});
    left.end->edges.push_back(Edge{'\0', right.end});

    start = right.start;
    end = right.end;
    end->stateInfo = _info;
}

void EpsilonNFA::NFAGenerator::visit(Kleene& node)
{
    NFAGenerator expr;
    node.expr()->accept(expr);

    adopt_pool(expr._pool);

    expr.end->edges.push_back(Edge{'\0', expr.start});
    expr.start->edges.push_back(Edge{'\0', expr.end});
    start = expr.start;
    end = expr.end;
    end->stateInfo = _info;
}

void EpsilonNFA::NFAGenerator::adopt_pool(vector<unique_ptr<Node>>& rhs)
{
    using iter_type = decltype(_pool)::iterator;
    _pool.insert(_pool.end(),
                 std::move_iterator<iter_type>(rhs.begin()),
                 std::move_iterator<iter_type>(rhs.end()));
}

EpsilonNFA EpsilonNFA::generate(Regex& regex, const StateInfo& matchName)
{
    NFAGenerator generator(matchName);
    regex.regex()->accept(generator);
    return EpsilonNFA(generator);
}

EpsilonNFA EpsilonNFA::generate(const shared_ptr<IRegex>& regex, const StateInfo& matchName)
{
    NFAGenerator generator(matchName);
    regex->accept(generator);
    return EpsilonNFA(generator);
}

EpsilonNFA& EpsilonNFA::combine_regex(IRegex* regex, const StateInfo& matchName)
{
    NFAGenerator generator(matchName);
    regex->accept(generator);

    using iter_type = decltype(_pool)::iterator;
    _pool.insert(_pool.end(),
                 std::move_iterator<iter_type>(generator._pool.begin()),
                 std::move_iterator<iter_type>(generator._pool.end()));

    if (_startState == nullptr)
    {
        _startState = generator.start;
    }
    else
    {
        _startState->edges.push_back(Edge{'\0', generator.start});
    }
    _endStates.push_back(generator.end);
    return *this;
}

EpsilonNFA& EpsilonNFA::combine_regex(const string& regex, const StateInfo& matchName)
{
    RegexParser parser(regex);
    return combine_regex(parser.parse().get(), matchName);
}

EpsilonNFA& EpsilonNFA::combine_regex(Regex& regex, const StateInfo& matchName)
{
    return combine_regex(regex.regex(), matchName);
}

}
}
