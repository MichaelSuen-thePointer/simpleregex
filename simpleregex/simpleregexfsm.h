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
    vector<StateInfo> _endStates;
    int _dropState;
    int _invalidState;

    static tuple<vector<array<int, 256>>,
        vector<StateInfo>,
        int>
        generate(const DFA& dfa);
public:
    FSM()
    {
    }
    FSM(const DFA& dfa)
        : _stateMachine()
        , _endStates()
        , _invalidState()
    {
        auto paramPack = generate(dfa);
        _stateMachine = std::move(std::get<0>(paramPack));
        _endStates = std::move(std::get<1>(paramPack));
        _invalidState = std::get<2>(paramPack);
    }
    FSM(const FSM& fsm)
        : _stateMachine(fsm._stateMachine)
        , _endStates(fsm._endStates)
        , _invalidState(fsm._invalidState)
    {
    }
    FSM(const FSM&& fsm)
        : _stateMachine(std::move(fsm._stateMachine))
        , _endStates(std::move(fsm._endStates))
        , _invalidState(fsm._invalidState)
    {
    }
    FSM& operator=(const FSM& fsm)
    {
        _stateMachine = fsm._stateMachine;
        _endStates = fsm._endStates;
        _invalidState = fsm._invalidState;
        return *this;
    }
    FSM& operator=(FSM&& fsm)
    {
        _stateMachine = std::move(fsm._stateMachine);
        _endStates = std::move(fsm._endStates);
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
        for (auto iter = _endStates.begin(); iter != _endStates.end(); ++iter)
        {
            os << std::setw(3) << iter - _endStates.begin() << ": ";
            os << iter->label << ' ' << iter->name;
            os << '\n';
        }
    }
#endif //_DEBUG

    int state_count() { return _stateMachine.size(); }
    const vector<array<int, 256>>& state_machine() { return _stateMachine; }
    int invalid_state() { return _invalidState; }
    const vector<StateInfo>& end_states() { return _endStates; }
    int drop_state() { return _dropState; }
    void set_drop_state(const string& stateName)
    {
        for (auto iter = _endStates.begin(); iter != _endStates.end(); ++iter)
        {
            if (iter->name == stateName)
            {
                _dropState = iter - _endStates.begin();
                return;
            }
        }
        _dropState = -1;
    }

    StateInfo match(const string& text)
    {
        int lastState = 0;

        string::const_iterator toMatch = text.cbegin();

        while (toMatch != text.end() && lastState != _invalidState)
        {
            lastState = _stateMachine[lastState][*toMatch++];
        }
        if (toMatch == text.end())
        {
            if (_endStates[lastState].is_end_state())
            {
                return _endStates[lastState];
            }
        }
        return StateInfo();
    }
};

}
}
#endif // !SIMPLE_REGEX_FSM
