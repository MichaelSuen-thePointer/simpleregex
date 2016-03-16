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
using std::shared_ptr;
using std::string;

class Empty;
class Char;
class Concat;
class Or;
class Kleene;

class IRegex
{
public:
    virtual ~IRegex() = default;
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

class Empty: public IRegex
{
public:
    virtual bool match(iterator&) override;
    virtual void accept(IVisitor&) override;
};

class Char: public IRegex
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

class Concat: public IRegex
{
protected:
    unique_ptr<IRegex> _left;
    unique_ptr<IRegex> _right;
public:
    Concat(IRegex* left, IRegex* right)
        : _left(left)
        , _right(right)
    {
    }
    Concat(unique_ptr<IRegex>&& left, unique_ptr<IRegex>&& right)
        : _left(std::move(left))
        , _right(std::move(right))
    {
    }
    virtual ~Concat()
    {
    }
    IRegex* left() { return _left.get(); }
    IRegex* right() { return _right.get(); }

    virtual bool match(iterator&) override;
    virtual void accept(IVisitor&) override;
};

class Or: public IRegex
{
protected:
    unique_ptr<IRegex> _left;
    unique_ptr<IRegex> _right;
public:
    Or(IRegex* left, IRegex* right)
        : _left(left)
        , _right(right)
    {
    }
    Or(unique_ptr<IRegex>&& left, unique_ptr<IRegex> right)
        : _left(std::move(left))
        , _right(std::move(right))
    {
    }
    virtual ~Or()
    {
    }

    IRegex* left() { return _left.get(); }
    IRegex* right() { return _right.get(); }
    virtual bool match(iterator& iter) override;
    virtual void accept(IVisitor& visitor) override;
};

class Kleene: public IRegex
{
protected:
    unique_ptr<IRegex> _expr;
public:
    Kleene(IRegex* expr)
        : _expr(expr)
    {
    }
    Kleene(unique_ptr<IRegex> expr)
        : _expr(std::move(expr))
    {
    }
    virtual ~Kleene()
    {
    }

    IRegex* expr() { return _expr.get(); }
    virtual bool match(iterator& iter) override;
    virtual void accept(IVisitor& visitor) override;
};


}
}

#endif // !SIMPLE_REGEX_H
