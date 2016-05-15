#include <iostream>
#include <string>
#include <memory>
#include <gtest/gtest.h>
#include "syntaxTreeComparer.h"

#include "..\simpleregex\simpleregexparser.h"

using namespace pl;

void gtest_token_eq(const regex::RegexTokenizer::RegexToken& a,
                    const regex::RegexTokenizer::RegexToken& b, int line)
{
    EXPECT_EQ(a.token, b.token) << "<-- Token at line" << line << '\n';
    EXPECT_EQ(a.type, b.type) << "<-- Type at line" << line << '\n';
}

#define GTEST_TOKEN_EQ(tokena, tokenb) gtest_token_eq(tokena, tokenb, __LINE__)

TEST(ParserTest, Tokenizer)
{
    using Token = regex::RegexTokenizer::RegexToken;
    using Type = regex::RegexTokenizer::RegexTokenType;
    const auto end = Token{ Type::EndToken, 0 };

    {
        std::string regex = R"__(ab(c)[a-z|A-Z]&*\n\t\a\b\(\)\[\]\*\|.\.)__";
        regex::RegexTokenizer tokenizer(regex);
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , 'a' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , 'b' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::LCircleBracket, '(' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , 'c' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::RCircleBracket, ')' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::LSquareBracket, '[' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , 'a' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Slash         , '-' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , 'z' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Alternative   , '|' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , 'A' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Slash         , '-' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , 'Z' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::RSquareBracket, ']' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , '&' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Star          , '*' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , '\n' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , '\t' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , 'a' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , 'b' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , '(' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , ')' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , '[' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , ']' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , '*' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , '|' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::Char          , '.' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::AllChar       , '.' }));
        GTEST_TOKEN_EQ(tokenizer.get(), (Token{ Type::EndToken      , 0 }));
    }
}

void gtest_tree_eq(regex::IRegex* a, regex::IRegex* b)
{
    GTestTreeComparer comparer(a);
    b->accept(comparer);
}

std::unique_ptr<regex::IRegex> alter(std::unique_ptr<regex::IRegex> a, std::unique_ptr<regex::IRegex> b)
{
    return std::make_unique<regex::Alternative>(a.release(), b.release());
}

std::unique_ptr<regex::IRegex> ch(char ch)
{
    return std::make_unique<regex::Char>(ch);
}

std::unique_ptr<regex::IRegex> con(std::unique_ptr<regex::IRegex> a, std::unique_ptr<regex::IRegex> b)
{
    return std::make_unique<regex::Concat>(a.release(), b.release());
}

std::unique_ptr<regex::IRegex> star(std::unique_ptr<regex::IRegex> a)
{
    return std::make_unique<regex::Kleene>(a.release());
}

std::unique_ptr<regex::IRegex> ran(char a, char b)
{
    return std::make_unique<regex::CharRange>(a, b);
}

TEST(ParserTest, Parser_CharRange)
{
    std::string regex = "[a-cE-G|a|b|c]";

    regex::RegexParser parser(regex);

    auto ansTree = parser.parse();
    auto stdTree = alter(
        ran('a', 'c'),
        alter(
            ran('E', 'G'),
            alter(
                ch('a'),
                alter(
                    ch('b'),
                    ch('c')
                )
            )
        )
    );

    gtest_tree_eq(ansTree.get(), stdTree.get());

}

TEST(ParserTest, Parser_Multiple)
{
    std::string regex = "([a-z|A-Z])a|b|cd*";

    regex::RegexParser parser(regex);

    auto ansTree = parser.parse();
    auto stdTree =
        alter(
            con(
                alter(
                    ran('a', 'z'),
                    ran('A', 'Z')),
                ch('a')),
            alter(
                ch('b'),
                con(
                    ch('c'),
                    star(ch('d')))));

    gtest_tree_eq(ansTree.get(), stdTree.get());
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);

    auto errcode = RUN_ALL_TESTS();

    std::cin.get();
    return errcode;
}