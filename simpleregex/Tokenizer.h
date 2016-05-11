#pragma once
#ifndef TOKENIZER_H
#define TOKENIZER_H
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
class Tokenizer
{
public:
    enum TokenKind
    {
        EndToken = 0,
        DROP = 1,
        LoadWord = 2,
        LoadByte = 3,
        LoadUByte = 4,
        LoadHalf = 5,
        LoadUHalf = 6,
        SaveWord = 7,
        SaveByte = 8,
        SaveHalf = 9,
        Add = 10,
        AddU = 11,
        Sub = 12,
        SubU = 13,
        SetLess = 14,
        SetULess = 15,
        And = 16,
        Alternative = 17,
        Xor = 18,
        Nor = 19,
        LogicLShift = 20,
        LogicRShift = 21,
        ArithRShift = 22,
        Mul = 23,
        MulU = 24,
        Div = 25,
        DivU = 26,
        InstAdd = 27,
        InstAddU = 28,
        InstAnd = 29,
        InstAndU = 30,
        InstOr = 31,
        InstXor = 32,
        InstLoadU = 33,
        InstSetLess = 34,
        InstSetULess = 35,
        JumpE = 36,
        JumpNE = 37,
        JumpLE = 38,
        JumpG = 39,
        JumpL = 40,
        JumpGE = 41,
        Jump = 42,
        JumpLink = 43,
        JumpAlR = 44,
        JumpR = 45,
        MFHI = 46,
        MGLO = 47,
        MTHI = 48,
        MTLO = 49,
        ErrorReturn = 50,
        MoveFromCo0 = 51,
        MoveToCo0 = 52,
        Break = 53,
        SysCall = 54,
        LoadAddr = 55,
        Move = 56,
        InstLoad = 57,
        Label = 58,
    };
    struct Token
    {
        TokenKind kind;
        std::string content;
        int line, column;
    };
protected:
    static int _stateMachine[138][256];
    static int _invalidState;
    static int _endStates[137];
    static int _dropState;
    int _lastState;
    int _line, _column;
    std::ifstream _file;
    Token _prefetch;
public:
    Tokenizer(const std::string& file)
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