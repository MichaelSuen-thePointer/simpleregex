#pragma once

#ifndef SIMPLE_REGEX_NFA
#define SIMPLE_REGEX_NFA

#include <set>
#include <map>
#include <functional>
#include <queue>
#include "simpleregexepsilonnfa.h"
#include "distinct_queue.h"

namespace pl
{
namespace regex
{

using std::set;
using std::function;
using std::map;
using std::queue;
using std::pair;
using container::distinct_queue;

class NFA
{
protected:
    vector<unique_ptr<Node>> _pool;
    Node* _start;

    static void BFS_search(const EpsilonNFA& enfa, const function<void(Node*)> apply);

    static set<Node*> find_valid_states(const EpsilonNFA& enfa);

    static set<const Node*> find_epsilon_closure(const Node* node);

private:
    using state_pair = pair<Node*, string::const_iterator>;

    struct state_pair_equal
    {
        bool operator()(const state_pair& lhs, const state_pair& rhs) const
        {
            return lhs.first == rhs.first && lhs.second == rhs.second;
        }
    };

    distinct_queue<state_pair, state_pair_equal> _matchStates;
    vector<string> _results;

    void clear_states()
    {
        _matchStates.clear();
    }

    bool unguarded_match(const string& text);
public:
    NFA(const EpsilonNFA& enfa)
    {
        auto nfa = generate(enfa);
        _pool = std::move(nfa.first);
        _start = nfa.second;
    }

    Node* start() { return _start; }

    const Node* start() const { return _start; }

    static pair<vector<unique_ptr<Node>>, Node*> generate(const EpsilonNFA& enfa);

    vector<string> match_all(const string& text);

    string match_first(const string& text);

    bool can_match()
    {
        return _matchStates.size() > 0;
    }
    void debug_print(std::ostream& os)
    {
        map<Node*, bool> visited;
        queue<Node*> toVisit;
        toVisit.push(_start);
        while (toVisit.size())
        {
            Node* current = toVisit.front();
            
            os << current << "\t";
            os << (current->stateName.size() ? current->stateName : "\t" ) << "\n";

            visited[current] = true;
            for (const auto& edge : current->edges)
            {
                os << "\t" << edge.accept << " S->" << edge.next<< "\n";

                if (visited.find(edge.next) == visited.end())
                {
                    toVisit.push(edge.next);
                }
            }
            toVisit.pop();
        }
    }
};


}
}
#endif // !SIMPLE_REGEX_NFA
