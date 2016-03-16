#include "simpleregexepsilonnfa.h"
namespace pl
{
namespace regex
{

void EpsilonNFA::NFAGenerator::visit(Empty&)
{
    end = start;
    end->stateName = _name;
}

void EpsilonNFA::NFAGenerator::visit(Char& node)
{
    _pool.push_back(std::make_unique<Node>());
    auto newNode = _pool.back().get();
    start->edges.push_back(Edge{node.ch(), newNode});
    end = newNode;
    end->stateName = _name;
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
    end->stateName = _name;
}

void EpsilonNFA::NFAGenerator::visit(Or& node)
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
    end->stateName = _name;
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
    end->stateName = _name;
}

void EpsilonNFA::NFAGenerator::adopt_pool(vector<unique_ptr<Node>>& rhs)
{
    using iter_type = decltype(_pool)::iterator;
    _pool.insert(_pool.end(),
                 std::move_iterator<iter_type>(rhs.begin()),
                 std::move_iterator<iter_type>(rhs.end()));
}

EpsilonNFA EpsilonNFA::generate(shared_ptr<Regex> regex, string matchName)
{
    NFAGenerator generator(matchName);
    regex->accept(generator);
    return EpsilonNFA(std::move(generator._pool), generator.start, generator.end);
}

EpsilonNFA& EpsilonNFA::combine_regex(shared_ptr<Regex> regex, string matchName)
{
    NFAGenerator generator(matchName);
    regex->accept(generator);
    
    using iter_type = decltype(_pool)::iterator;
    _pool.insert(_pool.end(),
                 std::move_iterator<iter_type>(generator._pool.begin()),
                 std::move_iterator<iter_type>(generator._pool.end()));

    _startState->edges.push_back(Edge{'\0', generator.start});
    _endStates.push_back(generator.end);
    return *this;
}

}
}
