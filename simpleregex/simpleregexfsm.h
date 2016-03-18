#pragma once
#ifndef SIMPLE_REGEX_FSM
#define SIMPLE_REGEX_FSM

#include <array>
#include <deque>
#include <map>
#include <tuple>
#ifdef _DEBUG
#include <iomanip>
#endif //_DEBUG
#include "simpleregexdfa.h"

namespace pl
{
namespace regex
{

using std::vector;
using std::array;
using std::priority_queue;
using std::string;
using std::tuple;

class FSM
{
protected:
    vector<array<int, 256>> _stateMachine;
    map<int, vector<string>> _endState;
    int _invalidState;

    static tuple<vector<array<int, 256>>,
        map<int, vector<string>>,
        int>
        generate(const DFA& dfa);
public:
    FSM()
    {
    }
    FSM(const DFA& dfa)
        : _stateMachine()
        , _endState()
        , _invalidState()
    {
        auto paramPack = generate(dfa);
        _stateMachine = std::move(std::get<0>(paramPack));
        _endState = std::move(std::get<1>(paramPack));
        _invalidState = std::get<2>(paramPack);
    }
    FSM(const FSM& fsm)
        : _stateMachine(fsm._stateMachine)
        , _endState(fsm._endState)
        , _invalidState(fsm._invalidState)
    {
    }
    FSM(const FSM&& fsm)
        : _stateMachine(std::move(fsm._stateMachine))
        , _endState(std::move(fsm._endState))
        , _invalidState(fsm._invalidState)
    {
    }
    FSM& operator=(const FSM& fsm)
    {
        _stateMachine = fsm._stateMachine;
        _endState = fsm._endState;
        _invalidState = fsm._invalidState;
        return *this;
    }
    FSM& operator=(FSM&& fsm)
    {
        _stateMachine = std::move(fsm._stateMachine);
        _endState = std::move(fsm._endState);
        _invalidState = fsm._invalidState;
        return *this;
    }

#ifdef _DEBUG
    void debug_print(std::ostream& os)
    {
        os << std::setw(3) << "";
        for (int i = 0; i < 256; i++)
        {
            if (isprint(i))
            {
                os << std::setw(3) << (char)i;
            }
            else
            {
                os << std::setw(3) << std::hex << i << std::dec;
            }
        }
        os << '\n';
        for (size_t i = 0; i != _stateMachine.size(); i++)
        {
            os << std::setw(3) << i;
            for (size_t j = 0; j != _stateMachine[i].size(); j++)
            {
                os << std::setw(3) << _stateMachine[i][j];
            }
            os << '\n';
        }
        for (auto& pair : _endState)
        {
            os << std::setw(3) << pair.first << ": ";
            std::copy(pair.second.begin(), pair.second.end(), std::ostream_iterator<string>(os, " "));
            os << '\n';
        }
    }
#endif //_DEBUG

    int state_count() { return _stateMachine.size(); }
    const vector<array<int, 256>>& state_machine(){return _stateMachine; }
    int invalid_state() { return _invalidState; }

    string try_end_state(int state)
    {
        auto place = _endState.find(state);
        if (place == _endState.end())
        {
            return "";
        }
        return place->second.front();
    }

    vector<string> match(const string& text)
    {
        int lastState = 0;

        string::const_iterator toMatch = text.cbegin();

        while (toMatch != text.end() && lastState != _invalidState)
        {
            lastState = _stateMachine[lastState][*toMatch++];
        }
        if (toMatch == text.end())
        {
            if (_endState.find(lastState) != _endState.end())
            {
                return _endState[lastState];
            }
        }
        return vector<string>();
    }
};

}
}
#endif // !SIMPLE_REGEX_FSM
