#pragma once
#ifndef SIMPLE_REGEX_PARSER_H
#define SIMPLE_REGEX_PARSER_H

#include "simpleregexdecl.h"
#include <stdexcept>
#include <initializer_list>
#include <sstream>
namespace pl
{
namespace regex
{
using std::shared_ptr;
using std::string;

class ParseError: std::runtime_error
{
public:
    char error_char;
    char expect;
    ParseError(const char* message, const char error, const char _expect)
        : std::runtime_error(message)
        , error_char(error)
        , expect(_expect)
    {
    }
};

class BadToken: std::runtime_error
{
public:
    char bad_token;
    BadToken(const char* message, const char token)
        : std::runtime_error(message)
        , bad_token(token)
    {
    }
};

class RegexTokenizer
{
protected:
    std::istringstream _stream;
    int _prefetch;
    int generate_token(int ch)
    {
        if (ch == '\\')
        {
            ch = _stream.get();
            if (ch == '*' || ch == '|' || ch == '(' || ch == ')' || ch == '\\')
            {
                return ch;
            }
            if (ch == 't')
            {
                return '\t';
            }
            if (ch == 'n')
            {
                return '\n';
            }
            else
            {
                throw BadToken("bad escaping character", ch);
            }
        }
        if (ch == '*')
        {
            return STAR;
        }
        if (ch == '|')
        {
            return OR;
        }
        if (ch == '(')
        {
            return LBRACE;
        }
        if (ch == ')')
        {
            return RBRACE;
        }
        if (isprint(ch))
        {
            return ch;
        }
        throw BadToken("bad token", ch);
    }

    void generate_prefetch()
    {
        if (_stream.peek() != -1)
        {
            _prefetch = generate_token(_stream.get());
        }
        else
        {
            _prefetch = END;
        }
    }


public:
    enum TokenType: int
    {
        END = 0,
        STAR = 256,
        OR = 257,
        LBRACE = 258,
        RBRACE = 259,
    };

    RegexTokenizer()
        : _stream()
        , _prefetch()
    {
    }

    RegexTokenizer(string reg_expr)
        : _stream(reg_expr)
        , _prefetch()
    {
        generate_prefetch();
    }
    void reset(string regexp)
    {
        _stream.str(regexp);
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
    int get()
    {
        int result = _prefetch;
        generate_prefetch();

        return result;
    }
    int peek()
    {
        return _prefetch;
    }
    static bool is_char(int ch)
    {
        if (ch >= 0 && ch <= 255)
        {
            return isprint(ch) != 0;
        }
        return false;
    }
};

class RegexParser
{
private:
    RegexTokenizer _token_stream;

    static void expect(int ch, int expect)
    {
        if (ch != expect)
        {
            throw ParseError("token not expected", ch, expect);
        }
    }
    static void not_expect(int ch, int nexpect)
    {
        if (ch == nexpect)
        {
            throw ParseError("token not expected", ch, nexpect);
        }
    }
    static void expect_any(int ch, std::initializer_list<int> list)
    {
        if (std::find(list.begin(), list.end(), ch) == list.end())
        {
            throw ParseError("didn't meet any of", ch, '\0');
        }
    }
    static void not_expect_any(int ch, std::initializer_list<int> list)
    {
        if (std::find(list.begin(), list.end(), ch) != list.end())
        {
            throw ParseError("meet any of", ch, '\0');
        }
    }
private:

    using Token = RegexTokenizer;

    unique_ptr<Regex> parse_char()
    {
        unique_ptr<Regex> result;
        int token = _token_stream.get();
        if (Token::is_char(token))
        {
            result = std::make_unique<Char>(token);
        }
        else if (token == Token::LBRACE)
        {
            result = parse_expr();
            expect(_token_stream.get(), Token::RBRACE);
        }
        token = _token_stream.peek();
        if (token == Token::STAR)
        {
            _token_stream.get();
            result = std::make_unique<Kleene>(result.release());
        }
        return result;
    }

    unique_ptr<Regex> parse_or_term()
    {
        unique_ptr<Regex> result;
        int token;
        result = parse_char();
        token = _token_stream.peek();
        if (token == Token::OR)
        {
            _token_stream.get();
            auto another = parse_or_term();
            result = std::make_unique<Or>(result.release(), another.release());
        }
        return result;
    }

    unique_ptr<Regex> parse_concat_term()
    {
        unique_ptr<Regex> result;
        result = parse_or_term();
        int token = _token_stream.peek();
        if (Token::is_char(token))
        {
            auto another = parse_concat_term();
            result = std::make_unique<Concat>(result.release(), another.release());
        }
        else if (token != Token::END && token != Token::RBRACE)
        {
            throw ParseError("unexpected token", token, '\0');
        }
        return result;
    }

    unique_ptr<Regex> parse_expr()
    {
        if (_token_stream.peek() != Token::END)
        {
            return parse_concat_term();
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
    shared_ptr<Regex> parse(string expr)
    {
        reset(expr);
        return parse();
    }
    shared_ptr<Regex> parse()
    {
        reset();
        return shared_ptr<Regex>(parse_expr());
    }
};


}
};

#endif // !SIMPLE_REGEX_PARSER_H
