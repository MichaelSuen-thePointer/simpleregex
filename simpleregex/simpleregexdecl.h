#pragma once
#ifndef SIMPLE_REGEX_DECL_H
#define SIMPLE_REGEX_DECL_H

#include <string>
#include <memory>

namespace pl
{
namespace regex
{

using iterator = std::string::iterator;
using std::unique_ptr;

class Empty;
class Char;
class Concat;
class Or;
class Kleene;

class Regex
{
public:
    virtual ~Regex() {}
    virtual bool match(iterator&) = 0;
    
    class IVisitor
    {
    private:
        IVisitor(const IVisitor&) = delete;
        IVisitor& operator=(const IVisitor&) = delete;
    public:
        IVisitor() {}
        virtual ~IVisitor() {}
        virtual void visit(Empty&) = 0;
        virtual void visit(Char&) = 0;
        virtual void visit(Concat&) = 0;
        virtual void visit(Or&) = 0;
        virtual void visit(Kleene&) = 0;
    };

    virtual void accept(IVisitor&) = 0;
};

class Empty: public Regex
{
public:
    virtual bool match(iterator&) override
    {
        return true;
    }
    virtual void accept(IVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};

class Char: public Regex
{
protected:
    char _ch;
public:
    Char(char ch)
        : _ch(ch)
    {
    }
    virtual ~Char()
    {
    }
    char ch() { return _ch; }
    virtual bool match(iterator& iter) override
    {
        if (*iter == _ch)
        {
            iter++;
            return true;
        }
        return false;
    }
    virtual void accept(IVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};

class Concat: public Regex
{
protected:
    unique_ptr<Regex> _left;
    unique_ptr<Regex> _right;
public:
    Concat(Regex* left, Regex* right)
        : _left(left)
        , _right(right)
    {
    }
    virtual ~Concat()
    {
    }
    const unique_ptr<Regex>& left() { return _left; }
    const unique_ptr<Regex>& right() { return _right; }

    virtual bool match(iterator& iter) override
    {
        iterator backUp = iter;
        if (_left->match(iter) && _right->match(iter))
        {
            return true;
        }
        iter = backUp;
        return false;
    }
    virtual void accept(IVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};

class Or: public Regex
{
protected:
    unique_ptr<Regex> _left;
    unique_ptr<Regex> _right;
public:
    Or(Regex* left, Regex* right)
        : _left(left)
        , _right(right)
    {
    }
    virtual ~Or()
    {
    }

    const unique_ptr<Regex>& left() { return _left; }
    const unique_ptr<Regex>& right() { return _right; }
    virtual bool match(iterator& iter) override
    {
        if (_left->match(iter) || _right->match(iter))
        {
            return true;
        }
        return false;
    }
    virtual void accept(IVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};

class Kleene: public Regex
{
protected:
    unique_ptr<Regex> _expr;
public:
    Kleene(Regex* expr)
        : _expr(expr)
    {
    }
    virtual ~Kleene()
    {
    }

    const unique_ptr<Regex>& expr() { return _expr; }
    virtual bool match(iterator& iter) override
    {
        iterator backUp;
        do
        {
            backUp = iter;
        } while (_expr->match(iter));
        iter = backUp;
        return true;
    }
    virtual void accept(IVisitor& visitor) override
    {
        visitor.visit(*this);
    }
};

}
}

#endif // !SIMPLE_REGEX_H
