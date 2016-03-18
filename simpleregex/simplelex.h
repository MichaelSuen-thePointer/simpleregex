#pragma once
#ifndef SIMPLE_LEX_H
#define SIMPLE_LEX_H

#include "simpleregexfsm.h"
#include <stdexcept>
#include <fstream>

namespace pl
{
namespace lex
{
using namespace regex;
using std::vector;

class BadToken: std::runtime_error
{
public:
    int line, column;
    string content;
    BadToken(const string& _content, int _line, int _column)
        : std::runtime_error("bad token")
        , line(_line)
        , column(_column)
        , content(_content)
    {
    }
};

class Lex
{
private:
    FSM _fsm;
    int _line, _column;
    std::fstream _file;

public:
    enum class TokenKind
    {
        END
    };
    struct Token
    {
        TokenKind kind;
        string content;
        int line, column;
    };

    Token peek()
    {

    }

    Token match()
    {

    }

};

}
}
#endif // !SIMPLE_LEX_H
