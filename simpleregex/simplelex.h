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
    BadToken(char _content, int _line, int _column)
        : std::runtime_error("bad token")
        , line(_line)
        , column(_column)
        , content{_content}
    {
    }
};

class Lex
{
private:
    FSM _fsm;
    int _line, _column;
    std::fstream _file;
    int _lastState;
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
        string matched;
        while (_file)
        {
            char ch = _file.peek();
            if (ch == '\n')
            {
                _column++;
                _line = 0;
                continue;
            }
            _line++;
            if (_fsm.state_machine[_lastState][ch] != _fsm.invalid_state)
            {
                _lastState = _fsm.state_machine[_lastState][ch];
                matched.push_back(ch);
            }
            else if (matched.length() == 0)
            {
                throw BadToken(ch, _line, _column);
            }
            else
            {
                auto stateName = _fsm.try_end_state(_lastState);
                if (stateName == "")
                {
                    throw BadToken(matched, _line, _column);
                }
                _lastState = 0;
                _file.get();
                auto kind = get_token_kind(stateName);
                return Token{kind, matched, _line, _column};
            }
        }
        if (matched.length())
        {
            throw BadToken(matched, _line, _column);
        }
        return Token{TokenKind::END, matched, _line, _column};
    }
    TokenKind get_token_kind(const StateInfo& stateName)
    {
        return TokenKind(stateName.label);
    }

};

}
}
#endif // !SIMPLE_LEX_H
