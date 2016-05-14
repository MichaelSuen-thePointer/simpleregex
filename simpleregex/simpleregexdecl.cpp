#include "simpleregexdecl.h"

namespace pl
{
namespace regex
{

bool Empty::match(iterator&)
{
    return true;
}

void Empty::accept(IVisitor& visitor)
{
    visitor.visit(*this);
}

bool Char::match(iterator& iter)
{
    if (*iter == _ch)
    {
        iter++;
        return true;
    }
    return false;
}

void Char::accept(IVisitor& visitor)
{
    visitor.visit(*this);
}

bool Concat::match(iterator& iter)
{
    iterator backUp = iter;
    if (_left->match(iter)&& _right->match(iter))
    {
        return true;
    }
    iter = backUp;
    return false;
}

void Concat::accept(IVisitor& visitor)
{
    visitor.visit(*this);
}

bool Alternative::match(iterator& iter)
{
    if (_left->match(iter) || _right->match(iter))
    {
        return true;
    }
    return false;
}

void Alternative::accept(IVisitor& visitor)
{
    visitor.visit(*this);
}

bool Kleene::match(iterator& iter)
{
    iterator backUp;
    do
    {
        backUp = iter;
    } while (_expr->match(iter));
    iter = backUp;
    return true;
}

void Kleene::accept(IVisitor& visitor)
{
    visitor.visit(*this);
}

bool CharRange::match(iterator& iter)
{
    return in_range(*iter);
}

void CharRange::accept(IVisitor& visitor)
{
    visitor.visit(*this);
}

}
}
