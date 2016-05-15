#pragma once
#ifndef SIMPLE_REGEX_PARSER_H
#define SIMPLE_REGEX_PARSER_H

#include "simpleregexdecl.h"
#include <stdexcept>
#include <initializer_list>
#include <sstream>
#include <functional>
#include <cctype>

#if defined _DEBUG && defined PARSER_LOG

#include <fstream>

static std::ofstream logger("parser_log.txt");
#define LOGGER logger
#else

class NotingToLog
{
public:
    template<class T>
    NotingToLog& operator<<(T) { return *this; }
};

static NotingToLog logger;
#define LOGGER logger

#endif

namespace pl
{
namespace regex
{

using std::shared_ptr;
using std::string;

class BadRegexToken : std::runtime_error
{
public:
    char bad_token;
    std::streamoff position;
    BadRegexToken(const char* message, std::streamoff pos, const char token)
        : std::runtime_error(message)
        , bad_token(token)
        , position(pos)
    {
    }
};

class RegexTokenizer
{
public:
    enum class RegexTokenType : int
    {
        EndToken = 0,
        Star = 256,
        Alternative = 257,
        LCircleBracket = 258,
        RCircleBracket = 259,
        AllChar,
        //
        Char,
        LSquareBracket,
        RSquareBracket,
        Slash,
    };

    struct RegexToken
    {
        RegexTokenType type;
        char token;
    };
protected:

    enum class ParseState
    {
        None,
        InCharRange,
    };

    std::istringstream _stream;
    RegexToken _prefetch;

    RegexToken generate_token()
    {
        int ch = _stream.get();
        if (ch == '\\')
        {
            ch = _stream.get();
            if (ch == 't')
            {
                return{ RegexTokenType::Char, '\t' };
            }
            if (ch == 'n')
            {
                return{ RegexTokenType::Char, '\n' };
            }
            if (ch == '.')
            {
                return{ RegexTokenType::AllChar, (char)ch };
            }
            if (ch == -1)
            {
                throw BadRegexToken("meet end of input", _stream.tellg(), '\0');
            }
            else
            {
                return{ RegexTokenType::Char, (char)ch };
            }
        }
        switch (ch)
        {
        case '*':
        {
            return{ RegexTokenType::Star, (char)ch };
        }
        case '|':
        {
            return{ RegexTokenType::Alternative, (char)ch };
        }
        case '(':
        {
            return{ RegexTokenType::LCircleBracket, (char)ch };
        }
        case ')':
        {
            return{ RegexTokenType::RCircleBracket, (char)ch };
        }
        case '[':
        {
            return{ RegexTokenType::LSquareBracket, (char)ch };
        }
        case ']':
        {
            return{ RegexTokenType::RSquareBracket, (char)ch };
        }
        case '-':
        {
            return{ RegexTokenType::Slash, (char)ch };
        }
        case -1:
        {
            throw BadRegexToken("meet end of input", _stream.tellg(), '\0');
        }
        default:
        {
            if (std::isprint(ch))
            {
                return{ RegexTokenType::Char, (char)ch };
            }
            throw BadRegexToken("bad token", _stream.tellg(), (char)ch);
        }
        }
    }

    void generate_prefetch()
    {
        if (_stream.peek() != -1)
        {
            _prefetch = generate_token();
        }
        else
        {
            _prefetch = { RegexTokenType::EndToken, 0 };
        }
    }

public:
    RegexTokenizer()
        : _stream()
        , _prefetch{ RegexTokenType::EndToken, 0 }
    {
    }

    RegexTokenizer(string regex)
        : _stream(regex)
        , _prefetch{ RegexTokenType::EndToken, 0 }
    {
        generate_prefetch();
    }
    void reset(string regex)
    {
        _stream.str(regex);
        _stream.seekg(0, _stream.beg);
        _stream.clear();
        generate_prefetch();
    }
    void reset()
    {
        _stream.seekg(0, _stream.beg);
        _stream.clear();
        generate_prefetch();
    }
    RegexToken get()
    {
        auto result = _prefetch;
        generate_prefetch();
        return result;
    }
    RegexToken peek()
    {
        return _prefetch;
    }
};

class RegexParseError : std::runtime_error
{
public:
    RegexTokenizer::RegexToken error_token;
    RegexTokenizer::RegexTokenType expect_type;
    RegexParseError(const char* message, const RegexTokenizer::RegexToken& error, const RegexTokenizer::RegexTokenType expect)
        : std::runtime_error(message)
        , error_token(error)
        , expect_type(expect)
    {
    }
};

class RegexParser
{
private:
    using TokenType = RegexTokenizer::RegexTokenType;
    using Token = RegexTokenizer::RegexToken;

    RegexTokenizer _token_stream;

    Token stream_peek()
    {
        return _token_stream.peek();
    }

    Token stream_get()
    {
        return _token_stream.get();
    }

    static void expect_type(Token ch, TokenType expect)
    {
        if (ch.type != expect)
        {
            throw RegexParseError("token not expected", ch, expect);
        }
    }
    /*
    static void not_expect(int ch, int nexpect)
    {
        if (ch == nexpect)
        {
            throw RegexParseError("token not expected", ch, nexpect);
        }
    }
    static void expect_any(int ch, std::initializer_list<int> list)
    {
        if (std::find(list.begin(), list.end(), ch) == list.end())
        {
            throw RegexParseError("didn't meet any of", ch, '\0');
        }
    }
    static void not_expect_any(int ch, std::initializer_list<int> list)
    {
        if (std::find(list.begin(), list.end(), ch) != list.end())
        {
            throw RegexParseError("meet any of", ch, '\0');
        }
    }
    */
private:
    unique_ptr<IRegex> parse_escaping_all()
    {
        LOGGER << "escaping all\n";
        unique_ptr<IRegex> rhs;
        rhs = std::make_unique<Char>(9);
        for (int i = 11; i < 256; i++)
        {
            auto lhs = std::make_unique<Char>(i);
            auto expr = std::make_unique<Alternative>(lhs.release(), rhs.release());
            rhs.reset(expr.release());
        }
        return rhs;
    }

    unique_ptr<IRegex> parse_char_range_or_term()
    {
        Token token = stream_get();
        expect_type(token, TokenType::Char);
        char front = token.token;
        char back = front;
        if (stream_peek().type == TokenType::Slash)
        {
            stream_get();
            token = stream_get();
            expect_type(token, TokenType::Char);
            back = token.token;
            if (front > back)
            {
                throw RegexParseError("char range must be increasing", token, TokenType::Char);
            }
        }
        if (front == back)
        {
            return std::make_unique<Char>(front);
        }
        return std::make_unique<CharRange>(front, back);
    }

    unique_ptr<IRegex> parse_char_range() // parse [a|b|c-d] [a-zA-Z]
    {
        auto result = parse_char_range_or_term();
        while (stream_peek().type != TokenType::RSquareBracket)
        {
            if (stream_peek().type == TokenType::Alternative)
            {
                stream_get(); //allow [a|b|c]
            }
            auto nextTerm = parse_char_range();
            result = std::make_unique<Alternative>(result.release(), nextTerm.release());
        }
        return result;
    }

    unique_ptr<IRegex> parse_char()
    {
        LOGGER << "return char\n";
        unique_ptr<IRegex> result;
        Token token = _token_stream.get();

        switch (token.type)
        {
        case TokenType::AllChar:
        {
            result = parse_escaping_all();
            break;
        }
        case TokenType::Char:
        {
            result = std::make_unique<Char>(token.token);
            break;
        }
        case TokenType::LSquareBracket:
        {
            result = parse_char_range();
            expect_type(stream_get(), TokenType::RSquareBracket);
            break;
        }
        case TokenType::LCircleBracket:
        {
            result = parse_expr();
            expect_type(stream_get(), TokenType::RCircleBracket);
            break;
        }
        default:
        {
            throw RegexParseError("expect char, char range or expression here", token, TokenType::Char);
        }
        }
        token = stream_peek();
        if (token.type == TokenType::Star)
        {
            stream_get();
            result = std::make_unique<Kleene>(result.release());
        }
        return result;
    }

    unique_ptr<IRegex> parse_concat_term()
    {
        unique_ptr<IRegex> result = parse_char();
        Token token = stream_peek();
        if (token.type == TokenType::Char || token.type == TokenType::LCircleBracket)
        {
            auto another = parse_concat_term();
            result = std::make_unique<Concat>(result.release(), another.release());
        }
        return result;
    }

    unique_ptr<IRegex> parse_or_term()
    {
        unique_ptr<IRegex> result = parse_concat_term();
        Token token = stream_peek();
        if (token.type == TokenType::Alternative)
        {
            stream_get();
            auto another = parse_or_term();
            result = std::make_unique<Alternative>(result.release(), another.release());
        }
        else if (token.type != TokenType::EndToken && token.type != TokenType::RCircleBracket)
        {
            throw RegexParseError("unexpected token", token, TokenType::Alternative);
        }
        return result;
    }

    unique_ptr<IRegex> parse_expr()
    {
        if (stream_peek().type != TokenType::EndToken)
        {
            return parse_or_term();
        }
        return std::make_unique<Empty>();
    }
public:
    RegexParser()
        : _token_stream()
    {
    }
    RegexParser(string regexp)
        : _token_stream(regexp)
    {
    }
    void reset(string regexp)
    {
        _token_stream.reset(regexp);
    }
    void reset()
    {
        _token_stream.reset();
    }
    unique_ptr<IRegex> parse(string expr)
    {
        reset(expr);
        return parse();
    }
    unique_ptr<IRegex> parse()
    {
        reset();
        return parse_expr();
    }
};


}
};

#endif // !SIMPLE_REGEX_PARSER_H
