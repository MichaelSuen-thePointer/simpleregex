#include "simpleregexepsilonnfa.h"
namespace pl
{
namespace regex
{

void EpsilonNFA::NFAGenerator::visit(Empty&)
{
    shared_ptr<Node> newNode = std::make_shared<Node>();
    start->strongEdges.push_back(StrongEdge{'\0', newNode});
    end = newNode;
    end->stateName = _name;
}

void EpsilonNFA::NFAGenerator::visit(Char& node)
{
    shared_ptr<Node> newNode = std::make_shared<Node>();
    start->strongEdges.push_back(StrongEdge{node.ch(), newNode});
    end = newNode;
    end->stateName = _name;
}

void EpsilonNFA::NFAGenerator::visit(Concat& node)
{
    NFAGenerator left, right;
    node.left()->accept(left);
    node.right()->accept(right);
    start = left.start;
    left.end->strongEdges.push_back(StrongEdge{'\0', right.start});
    end = right.end;
    end->stateName = _name;
}

void EpsilonNFA::NFAGenerator::visit(Or& node)
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

void EpsilonNFA::NFAGenerator::visit(Kleene& node)
{
    NFAGenerator expr;
    node.expr()->accept(expr);
    expr.end->weakEdges.push_back(WeakEdge{'\0', expr.start});
    expr.start->weakEdges.push_back(WeakEdge{'\0', expr.end});
    start = expr.start;
    end = expr.end;
    end->stateName = _name;
}

EpsilonNFA EpsilonNFA::generate(shared_ptr<Regex> regex, string matchName)
{
    NFAGenerator generator(matchName);
    regex->accept(generator);
    return EpsilonNFA(generator.start, generator.end);
}

EpsilonNFA& EpsilonNFA::combine_regex(shared_ptr<Regex> regex, string matchName)
{
    NFAGenerator generator(matchName);
    regex->accept(generator);
    startState->strongEdges.push_back(StrongEdge{'\0', generator.start});
    endStates.push_back(generator.end);
    return *this;
}

}
}
