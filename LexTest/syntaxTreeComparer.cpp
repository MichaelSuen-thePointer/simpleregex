#include "syntaxTreeComparer.h"

using namespace pl::regex;

void GTestTreeComparer::visit(Empty& expr)
{

}

void GTestTreeComparer::visit(Char& expr)
{
    GTestComparer<typename std::remove_reference<decltype(expr)>::type> comparer(&expr);
    node->accept(comparer);
}

void GTestTreeComparer::visit(CharRange& expr)
{
    GTestComparer<typename std::remove_reference<decltype(expr)>::type> comparer(&expr);
    node->accept(comparer);
}

void GTestTreeComparer::visit(Concat& expr)
{
    GTestComparer<typename std::remove_reference<decltype(expr)>::type> comparer(&expr);
    node->accept(comparer);
}

void GTestTreeComparer::visit(Alternative& expr)
{
    GTestComparer<typename std::remove_reference<decltype(expr)>::type> comparer(&expr);
    node->accept(comparer);
}

void GTestTreeComparer::visit(Kleene& expr)
{
    GTestComparer<typename std::remove_reference<decltype(expr)>::type> comparer(&expr);
    node->accept(comparer);
}
