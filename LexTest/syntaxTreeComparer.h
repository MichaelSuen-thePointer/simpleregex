#pragma once
#include <gtest/gtest.h>
#include "..\simpleregex\simpleregexparser.h"

class GTestCompareTree : public pl::regex::IRegex::IVisitor
{
public:
    virtual void visit(pl::regex::Empty& expr) override;
    virtual void visit(pl::regex::Char& expr) override;
    virtual void visit(pl::regex::CharRange& expr) override;
    virtual void visit(pl::regex::Concat& expr) override;
    virtual void visit(pl::regex::Alternative& expr) override;
    virtual void visit(pl::regex::Kleene& expr) override;
};

template<typename TRegexNode>
class GTestComparer : public pl::regex::IRegex::IVisitor
{
public:
    TRegexNode* node;
public:
    GTestComparer(TRegexNode* node)
        : node(node)
    {
    }
    virtual void visit(pl::regex::Empty& expr) override;
    virtual void visit(pl::regex::Char& expr) override;
    virtual void visit(pl::regex::CharRange& expr) override;
    virtual void visit(pl::regex::Concat& expr) override;
    virtual void visit(pl::regex::Alternative& expr) override;
    virtual void visit(pl::regex::Kleene& expr) override;
};

template<typename TRegexNode>
inline void GTestComparer<TRegexNode>::visit(pl::regex::Empty& expr)
{

}

template<typename TRegexNode>
inline void GTestComparer<TRegexNode>::visit(pl::regex::Char& expr)
{
}

template<typename TRegexNode>
inline void GTestComparer<TRegexNode>::visit(pl::regex::CharRange& expr)
{
}

template<typename TRegexNode>
inline void GTestComparer<TRegexNode>::visit(pl::regex::Concat& expr)
{
}

template<typename TRegexNode>
inline void GTestComparer<TRegexNode>::visit(pl::regex::Alternative& expr)
{
}

template<typename TRegexNode>
inline void GTestComparer<TRegexNode>::visit(pl::regex::Kleene& expr)
{
}
