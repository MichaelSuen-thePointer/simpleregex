#include "simpleregex.h"

namespace pl
{
namespace regex
{

void Regex::Cloner::visit(Empty& expr)
{
    _result = std::make_unique<Empty>();
}

void Regex::Cloner::visit(Char& expr)
{
    _result = std::make_unique<Char>(expr.ch());
}

void Regex::Cloner::visit(Concat& expr)
{
    Cloner left, right;
    expr.left()->accept(left);
    expr.right()->accept(right);

    _result = std::make_unique<Concat>(left.move_result(), right.move_result());
}

void Regex::Cloner::visit(Alternative& expr)
{
    Cloner left, right;
    expr.left()->accept(left);
    expr.right()->accept(right);

    _result = std::make_unique<Alternative>(left.move_result(), right.move_result());
}

void Regex::Cloner::visit(Kleene& expr)
{
    Cloner cloner;
    expr.expr()->accept(cloner);

    _result = std::make_unique<Kleene>(cloner.move_result());
}

}
}