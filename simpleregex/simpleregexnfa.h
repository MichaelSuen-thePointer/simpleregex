#pragma once

#ifndef SIMPLE_REGEX_NFA
#define SIMPLE_REGEX_NFA

#include <memory>
#include "simpleregexepsilonnfa.h"

namespace pl
{
namespace regex
{


class NFA
{
protected:
    shared_ptr<Node> startState;
    vector<shared_ptr<Node>> endStates;
public:
    static NFA generate(const EpsilonNFA& enfa)
    {

    }

};


}
}
#endif // !SIMPLE_REGEX_NFA
