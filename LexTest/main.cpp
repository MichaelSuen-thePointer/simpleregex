#include <iostream>
#include <string>
#include <gtest/gtest.h>

#include "..\simpleregex\simpleregexparser.h"

using namespace pl;

void gtest_token_eq(const regex::RegexTokenizer::RegexToken& a,
                    const regex::RegexTokenizer::RegexToken& b, int line)
{
    EXPECT_EQ(a.token, b.token) << "<-- Token at line" << line << '\n';
    EXPECT_EQ(a.type, b.type) << "<-- Type at line" << line << '\n';
}

TEST(ParserTest, Tokenizer)
{
    using Token = regex::RegexTokenizer::RegexToken;
    using Type = regex::RegexTokenizer::RegexTokenType;
    const auto end = Token{ Type::EndToken, 0 };

    {
        std::string regex = R"__(ab(c)[a-z|A-Z]&*\n\t\a\b\(\)\[\]\*\|.\.)__";
        regex::RegexTokenizer tokenizer(regex);

        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , 'a' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , 'b' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::LCircleBracket, '(' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , 'c' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::RCircleBracket, ')' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::LSquareBracket, '[' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , 'a' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Slash         , '-' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , 'z' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Alternative   , '|' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , 'A' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Slash         , '-' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , 'Z' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::RSquareBracket, ']' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , '&' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Star          , '*' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , '\n' } , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , '\t' } , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , 'a' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , 'b' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , '(' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , ')' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , '[' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , ']' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , '*' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , '|' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::Char          , '.' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::AllChar       , '.' }  , __LINE__);
        gtest_token_eq(tokenizer.get(), Token{ Type::EndToken      , 0 }    , __LINE__);
    }
}

TEST(ParserTest, Parser_Single)
{
    
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);

    auto errcode = RUN_ALL_TESTS();

    std::cin.get();
    return errcode;
}