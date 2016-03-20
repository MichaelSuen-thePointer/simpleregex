#pragma once
#ifndef LEXREADERTOKENIZER_H
#define LEXREADERTOKENIZER_H
#include <fstream>
#include <string>
#include <map>
class BadToken: std::runtime_error
{
public:
    int line, column;
    std::string content;
    BadToken(const std::string& _content, int _line, int _column)
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
struct StateInfo
{
    int label;
    std::string name;
    StateInfo(int _label = -1, std::string _name = "")
        : label(_label)
        , name(_name)
    {
    }
};
class LexReaderTokenizer
{
public:
    enum TokenKind
    {
        End = 0,
         = 1,
         = 2,
         = 3,
         = 4,
         = 5,
    };
    struct Token
    {
        TokenKind kind;
        std::string content;
        int line, column;
    };
protected:
    static int _stateMachine[152][256];
    static int _invalidState;
    static int _endStates[151];
    static int _dropState;
    int _lastState;
    int _line, _column;
    std::ifstream _file;
    Token _prefetch;
public:
    LexReaderTokenizer(const std::string& file)
        : _file(file)
        , _line(1)
    {
        prefetch();
    }
    Token get();
    Token peek();
private:
    void prefetch();
};
#endif