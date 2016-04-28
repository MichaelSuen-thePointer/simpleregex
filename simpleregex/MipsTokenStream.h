#pragma once
#ifndef MIPSTOKENSTREAM_H
#define MIPSTOKENSTREAM_H
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
class MipsTokenStream
{
public:
    enum TokenKind
    {
        End = 0,
        COMMENT = 1,
        DROP = 2,
        LBRACE = 3,
        RBRACE = 4,
        LoadWord = 5,
        LoadByte = 6,
        LoadUByte = 7,
        LoadHalf = 8,
        LoadUHalf = 9,
        SaveWord = 10,
        SaveByte = 11,
        SaveHalf = 12,
        Add = 13,
        AddU = 14,
        Sub = 15,
        SubU = 16,
        SetLess = 17,
        SetULess = 18,
        And = 19,
        Or = 20,
        Xor = 21,
        Nor = 22,
        LogicLShift = 23,
        LogicRShift = 24,
        ArithRShift = 25,
        Mul = 26,
        MulU = 27,
        Div = 28,
        DivU = 29,
        InstAdd = 30,
        InstAddU = 31,
        InstAnd = 32,
        InstAndU = 33,
        InstOr = 34,
        InstXor = 35,
        InstLoadHi = 36,
        InstSetLess = 37,
        InstSetULess = 38,
        JumpE = 39,
        JumpNE = 40,
        JumpLEZ = 41,
        JumpGZ = 42,
        JumpLZ = 43,
        JumpGEZ = 44,
        Jump = 45,
        JumpLink = 46,
        JumpLinkReg = 47,
        JumpReg = 48,
        MFHI = 49,
        MFLO = 50,
        MTHI = 51,
        MTLO = 52,
        ErrorReturn = 53,
        MoveFromCo0 = 54,
        MoveToCo0 = 55,
        Break = 56,
        SysCall = 57,
        LoadAddr = 58,
        Move = 59,
        InstLoad = 60,
        DataAddress = 61,
        DefByte = 62,
        DefWord = 63,
        DefDword = 64,
        Equal = 65,
        ReserveByte = 66,
        ReserveWord = 67,
        ReserveDword; = 68,
        Comma = 69,
        InstrEnd = 70,
        HexNum = 71,
        DecNum = 72,
        OctNum = 73,
        CharArr = 74,
        Zero = 75,
        AsmTemp = 76,
        V0 = 77,
        V1 = 78,
        A0 = 79,
        A1 = 80,
        A2 = 81,
        A3 = 82,
        T0 = 83,
        T1 = 84,
        T2 = 85,
        T3 = 86,
        T4 = 87,
        T5 = 88,
        T6 = 89,
        T7 = 90,
        S0 = 91,
        S1 = 92,
        S2 = 93,
        S3 = 94,
        S4 = 95,
        S5 = 96,
        S6 = 97,
        S7 = 98,
        T8 = 99,
        T9 = 100,
        K0 = 101,
        K1 = 102,
        GlbPtr = 103,
        StackPtr = 104,
        FramePtr = 105,
        RetAddr = 106,
        Label = 107,
    };
    struct Token
    {
        TokenKind kind;
        std::string content;
        int line, column;
    };
protected:
    static int _stateMachine[904][256];
    static int _invalidState;
    static int _endStates[903];
    static int _dropState;
    int _lastState;
    int _line, _column;
    std::ifstream _file;
    Token _prefetch;
public:
    MipsTokenStream(const std::string& file)
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