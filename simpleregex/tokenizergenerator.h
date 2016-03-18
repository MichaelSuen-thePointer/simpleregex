#pragma once

#ifndef TOKENIZER_GENERATOR_H
#define TOKENIZER_GENERATOR_H

#include <fstream>
#include <algorithm>    
#include "simpleregexfsm.h"

namespace pl
{
namespace lex
{

using namespace regex;

class Generator
{
protected:
    std::ifstream _file;

    string className = "Tokenizer";
    vector<string> matchNames;
    EpsilonNFA enfa;
    FSM stateMachine;
public:
    Generator(const string& fileAddr)
        : _file(fileAddr)
        , enfa()
        , stateMachine()
    {
    }

    void generate()
    {
        constexpr int bufSize = 1024;
        char lineBuf[bufSize];

        while (_file.good())
        {
            _file.getline(lineBuf, bufSize, ':');
            string instructor(lineBuf);
            std::transform(instructor.begin(), instructor.end(), instructor.begin(), ::tolower);
            if (instructor == "name")
            {
                _file.getline(lineBuf, bufSize);
                if (strlen(lineBuf) != 0)
                {
                    className = lineBuf;
                }
                throw std::runtime_error("wrong class name");
            }
            if (instructor == "rules")
            {
                while (_file.good())
                {

                    _file.getline(lineBuf, bufSize);
                    string line(lineBuf);

                    auto iter = line.end();
                    for (; iter[-1] != ' '; iter--)
                    {
                        continue;
                    }
                    iter--;
                    string regex(line.begin(), iter);
                    string name(iter + 1, line.end());
                    matchNames.push_back(name);
                    enfa.combine_regex(regex, name);
                }
                break;
            }
        }
        NFA nfa(enfa);
        DFA dfa(nfa);
        stateMachine = FSM(dfa);

        std::ofstream header(className + ".h");
        header << R"__(
#pragma once
#ifndef )__" << className << "_H" << R"__(
#define )__" << className << "_H" << R"__(

#include <ifstream>

namespace pl
{
namespace lex
{

class )__" << className << R"__(
{
public:
    enum class TokenKind
    {
        END)__";
        std::for_each(matchNames.begin(), matchNames.end(), [&header](const string& name) {
            header << ",\n" << name;
        });
        header << R"__(
    };
    
    struct Token
    {
        TokenKind kind,
        string content;
        int line, column
    };
private:
    static int _stateMachine[)__" << stateMachine.state_count() << "][255];" << R"__(
    int _currentState;
    int _line, _column;
    std::ifstream _file;
    Token _peek;

    )__" << className << R"__((const std::ifstream& file)
        : _currentState(0)
        , _line(1)
        , _column(1)
        , _file(file)
    {
        
    }
    void prefetch()
    {

    }
    


}
}
)__"

            
    }

};

}
}
#endif // !TOKENIZER_GENERATOR_H
