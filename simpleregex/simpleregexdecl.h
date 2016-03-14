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
    virtual bool match(iterator&) override;
    virtual void accept(IVisitor&) override;
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
    virtual ~Char() {}
    
    char ch() { return _ch; }

    virtual bool match(iterator&) override;
    virtual void accept(IVisitor&) override;
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
    Regex* left() { return _left.get(); }
    Regex* right() { return _right.get(); }

    virtual bool match(iterator&) override;
    virtual void accept(IVisitor&) override;
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

    Regex* left() { return _left.get(); }
    Regex* right() { return _right.get(); }
    virtual bool match(iterator& iter) override;
    virtual void accept(IVisitor& visitor) override;
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

    Regex* expr() { return _expr.get(); }
    virtual bool match(iterator& iter) override;
    virtual void accept(IVisitor& visitor) override;
};

}
}

#endif // !SIMPLE_REGEX_H
