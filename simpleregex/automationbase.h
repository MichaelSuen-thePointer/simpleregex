#pragma once
#ifndef AUTOMATION_BASE
#define AUTOMATION_BASE

#include "distinct_queue.h"

namespace pl
{
namespace regex
{

using container::distinct_queue;

template<class TMatchStatus, class TEqualCompare = std::equal_to<TMatchStatus>>
class MatchBase
{
protected:
    distinct_queue<TMatchStatus, TEqualCompare> _matchStatus;


};

}
}
#endif // !AUTOMATION_BASE
