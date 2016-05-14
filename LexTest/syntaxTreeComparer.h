#pragma once
#include <gtest/gtest.h>
#include <type_traits>
#include <string>
#include "..\simpleregex\simpleregexparser.h"

class GTestTreeComparer : public pl::regex::IRegex::IVisitor
{
public:
    pl::regex::IRegex* node;

    std::string trace;

    GTestTreeComparer(pl::regex::IRegex* expr, const std::string& trace = std::string())
        : node(expr)
        , trace(trace)
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
class GTestComparerBase : public pl::regex::IRegex::IVisitor
{
public:
    TRegexNode* node;
    std::string trace;
    GTestComparerBase(TRegexNode* _node, const std::string& _trace)
        : node(_node)
        , trace(_trace)
    {
    }
    virtual void visit(pl::regex::Empty& expr) override
    {
        ASSERT_STREQ(typeid(typename std::remove_pointer<decltype(node)>::type).name(),
                     typeid(typename std::remove_reference<decltype(expr)>::type).name())
            << trace;
    }
    virtual void visit(pl::regex::Char& expr) override
    {
        ASSERT_STREQ(typeid(typename std::remove_pointer<decltype(node)>::type).name(),
                     typeid(typename std::remove_reference<decltype(expr)>::type).name())
            << trace;
    }
    virtual void visit(pl::regex::CharRange& expr) override
    {
        ASSERT_STREQ(typeid(typename std::remove_pointer<decltype(node)>::type).name(),
                     typeid(typename std::remove_reference<decltype(expr)>::type).name())
            << trace;
    }
    virtual void visit(pl::regex::Concat& expr) override
    {
        ASSERT_STREQ(typeid(typename std::remove_pointer<decltype(node)>::type).name(),
                     typeid(typename std::remove_reference<decltype(expr)>::type).name())
            << trace;
    }
    virtual void visit(pl::regex::Alternative& expr) override
    {
        ASSERT_STREQ(typeid(typename std::remove_pointer<decltype(node)>::type).name(),
                     typeid(typename std::remove_reference<decltype(expr)>::type).name())
            << trace;
    }
    virtual void visit(pl::regex::Kleene& expr) override
    {
        ASSERT_STREQ(typeid(typename std::remove_pointer<decltype(node)>::type).name(),
                     typeid(typename std::remove_reference<decltype(expr)>::type).name())
            << trace;
    }
};

template<typename TRegexNode>
class GTestComparer;

template<>
class GTestComparer<pl::regex::Empty> : public GTestComparerBase<pl::regex::Empty>
{
public:
    using base = GTestComparerBase<pl::regex::Empty>;
    GTestComparer(pl::regex::Empty* node, std::string& trace = std::string())
        : base(node, trace)
    {
        this->trace += "in compare Empty node\n";
    }
    virtual void visit(pl::regex::Empty& expr) override;
};

template<>
class GTestComparer<pl::regex::Char> : public GTestComparerBase<pl::regex::Char>
{
public:
    using base = GTestComparerBase<pl::regex::Char>;
    GTestComparer(pl::regex::Char* node, std::string& trace = std::string())
        : base(node, trace)
    {
        this->trace += "in compare Char node\n";
    }
    virtual void visit(pl::regex::Char& expr) override;
};

template<>
class GTestComparer<pl::regex::CharRange> : public GTestComparerBase<pl::regex::CharRange>
{
public:
    using base = GTestComparerBase<pl::regex::CharRange>;
    GTestComparer(pl::regex::CharRange* node, std::string& trace = std::string())
        : base(node, trace)
    {
        this->trace += "in compare Char node\n";
    }
    virtual void visit(pl::regex::CharRange& expr) override;
};

template<>
class GTestComparer<pl::regex::Concat> : public GTestComparerBase<pl::regex::Concat>
{
public:
    using base = GTestComparerBase<pl::regex::Concat>;
    GTestComparer(pl::regex::Concat* node, std::string& trace = std::string())
        : base(node, trace)
    {
        this->trace += "in compare Concat node\n";
    }
    virtual void visit(pl::regex::Concat& expr) override;
};

template<>
class GTestComparer<pl::regex::Alternative> : public GTestComparerBase<pl::regex::Alternative>
{
public:
    using base = GTestComparerBase<pl::regex::Alternative>;
    GTestComparer(pl::regex::Alternative* node, std::string& trace = std::string())
        : base(node, trace)
    {
        this->trace += "in compare Alternative node\n";
    }
    virtual void visit(pl::regex::Alternative& expr) override;
};

template<>
class GTestComparer<pl::regex::Kleene> : public GTestComparerBase<pl::regex::Kleene>
{
public:
    using base = GTestComparerBase<pl::regex::Kleene>;
    GTestComparer(pl::regex::Kleene* node, std::string& trace = std::string())
        : base(node, trace)
    {
        this->trace += "in compare Kleene node\n";
    }
    virtual void visit(pl::regex::Kleene& expr) override;
};

inline void GTestComparer<pl::regex::Empty>::visit(pl::regex::Empty & expr)
{
}

inline void GTestComparer<pl::regex::Char>::visit(pl::regex::Char & expr)
{
    EXPECT_EQ(expr.ch(), node->ch()) << trace;
}

inline void GTestComparer<pl::regex::CharRange>::visit(pl::regex::CharRange & expr)
{
    EXPECT_EQ(expr.front(), node->front()) << trace << " right\n";
    EXPECT_EQ(expr.back(), node->back()) << trace << " left\n";
}

inline void GTestComparer<pl::regex::Concat>::visit(pl::regex::Concat & expr)
{
    GTestTreeComparer comparer1(expr.left(), trace + "in Concat left\n");
    node->left()->accept(comparer1);

    GTestTreeComparer comparer2(expr.right(), trace + "in Concat right\n");
    node->right()->accept(comparer2);
}

inline void GTestComparer<pl::regex::Alternative>::visit(pl::regex::Alternative & expr)
{
    GTestTreeComparer comparer1(expr.left(), trace + "in Alternative left\n");
    node->left()->accept(comparer1);

    GTestTreeComparer comparer2(expr.right(), trace + "in Alternative right\n");
    node->right()->accept(comparer2);
}

inline void GTestComparer<pl::regex::Kleene>::visit(pl::regex::Kleene & expr)
{
    GTestTreeComparer comparer(expr.expr(), trace + "in Kleene\n");
    node->expr()->accept(comparer);
}
