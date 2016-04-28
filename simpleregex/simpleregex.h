#pragma once
#ifndef SIMPLE_REGEX_H
#define SIMPLE_REGEX_H

#include <cassert>
#include "simpleregexparser.h"

namespace pl
{
namespace regex
{

using std::enable_if;
using std::is_same;
using std::decay;
using std::forward;

class Regex
{
protected:
    unique_ptr<IRegex> _regex;

    Regex(unique_ptr<IRegex>&& regex)
        : _regex(std::move(regex))
    {
    }

    class Cloner: public IRegex::IVisitor
    {
    public:
        unique_ptr<IRegex> move_result()
        {
            return std::move(_result);
        }
    private:
        unique_ptr<IRegex> _result;

        virtual void visit(Empty&) override;
        virtual void visit(Char&) override;
        virtual void visit(Concat&) override;
        virtual void visit(Or&) override;
        virtual void visit(Kleene&) override;
    };

    static unique_ptr<IRegex> copy_or_move(Regex& rhs)
    {
        Cloner cloner;
        rhs._regex->accept(cloner);
        return cloner.move_result();
    }

    static unique_ptr<IRegex> copy_or_move(Regex&& rhs)
    {
        return std::move(rhs._regex);
    }

    template<class T, class... TArgs>
    static Regex create(TArgs&&... args)
    {
        return Regex(std::make_unique<T>(std::forward<TArgs>(args)...));
    }

    template<class T, class... TArgs>
    Regex& update(TArgs&&... args)
    {
        _regex = std::make_unique<T>(std::move(_regex), std::forward<TArgs>(args)...);
        return *this;
    }
public:
    Regex(const Regex& other)
        : _regex()
    {
        Cloner cloner;
        other._regex->accept(cloner);
        _regex = cloner.move_result();
    }

    Regex(const string& text)
        : _regex(parse(text)._regex)
    {
    }

    static Regex parse(const string& regex)
    {
        RegexParser parser(regex);
        return Regex(parser.parse());
    }

    const IRegex* regex() const { return _regex.get(); }
    IRegex* regex() { return _regex.get(); }

    template<class Tl, class Tr, class, class> friend
    Regex operator+(Tl&& lhs, Tr&& rhs);

    template<class Tl, class Tr, class, class> friend
    Regex operator|(Tl&& lhs, Tr&& rhs);

    template<class Tl, class> friend
    Regex operator*(Tl&& lhs);

    template<class T, class = typename enable_if<is_same<typename decay<T>::type, Regex>::value>::type>
    Regex& operator+=(T&& rhs)
    {
        return update<Concat>(copy_or_move(std::forward<T>(rhs)));
    }
    
    template<class T, class = typename enable_if<is_same<typename decay<T>::type, Regex>::value>::type>
    Regex& operator|=(T&& rhs)
    {
        return update<Or>(copy_or_move(std::forward<T>(rhs)));
    }

    Regex& operator*=(int)
    {
        return update<Kleene>();
    }
};

template<class Tl, class Tr,
class = typename enable_if<is_same<typename decay<Tl>::type, Regex>::value>::type,
class = typename enable_if<is_same<typename decay<Tr>::type, Regex>::value>::type
>
Regex operator+(Tl&& lhs, Tr&& rhs)
{
    return Regex::create<Concat>(Regex::copy_or_move(forward<Tl>(lhs)), Regex::copy_or_move(forward<Tr>(rhs)));
}

template<class Tl, class Tr,
class = typename enable_if<is_same<typename decay<Tl>::type, Regex>::value>::type,
class = typename enable_if<is_same<typename decay<Tr>::type, Regex>::value>::type
>
Regex operator|(Tl&& lhs, Tr&& rhs)
{
    return Regex::create<Or>(Regex::copy_or_move(forward<Tl>(lhs)), Regex::copy_or_move(forward<Tr>(rhs)));
}

template<class Tl,
class = typename enable_if<is_same<typename decay<Tl>::type, Regex>::value>::type
>
Regex operator*(Tl&& lhs)
{
    return Regex::create<Kleene>(Regex::copy_or_move(forward<Tl>(lhs)));
}


}
}


#endif // !SIMPLE_REGEX_H
