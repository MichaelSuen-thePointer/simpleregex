#include "simpleregexnfa.h"
namespace pl
{
namespace regex
{
inline void NFA::BFS_search(const EpsilonNFA & enfa, const function<void(Node*)> apply)
{
    map<Node*, bool> visited;
    queue<Node*> toVisit;
    toVisit.push(enfa.start_state());
    while (toVisit.size())
    {
        Node* current = toVisit.front();
        apply(current);
        visited[current] = true;
        for (const auto& edge : current->strongEdges)
        {
            if (visited.find(edge.next.get()) == visited.end())
            {
                toVisit.push(edge.next.get());
            }
        }
        for (const auto& edge : current->weakEdges)
        {
            if (visited.find(edge.next.lock().get()) == visited.end())
            {
                toVisit.push(edge.next.lock().get());
            }
        }
        toVisit.pop();
    }
}
inline set<Node*> NFA::find_valid_states(const EpsilonNFA & enfa)
{
    set<Node*> validStates;
    auto apply = [&validStates](Node* node) {
        for (const auto& edge : node->strongEdges)
        {
            if (edge.accept != '\0')
            {
                validStates.insert(edge.next.get());
            }
        }
        for (const auto& edge : node->weakEdges)
        {
            if (edge.accept != '\0')
            {
                validStates.insert(edge.next.lock().get());
            }
        }
    };
    BFS_search(enfa, apply);
    validStates.insert(enfa.start_state());
    return validStates;
}
inline set<const Node*> NFA::find_epsilon_closure(const Node * node)
{
    map<const Node*, bool> visited;
    queue<const Node*> toVisit;
    set<const Node*> closure;
    toVisit.push(node);
    closure.insert(node);
    while (toVisit.size())
    {
        const Node* current = toVisit.front();
        visited[current] = true;
        for (const auto& edge : current->strongEdges)
        {
            auto next = edge.next.get();
            if (edge.accept == '\0' &&
                visited.find(next) == visited.end())
            {
                toVisit.push(next);
                closure.insert(next);
            }
        }
        for (const auto& edge : current->weakEdges)
        {
            auto next = edge.next.lock().get();
            if (edge.accept == '\0' &&
                visited.find(next) == visited.end())
            {
                toVisit.push(next);
                closure.insert(next);
            }
        }
        toVisit.pop();
    }
    return closure;
}
NFA NFA::generate(const EpsilonNFA & enfa)
{
    set<Node*> validStates = find_valid_states(enfa);
    map<const Node*, shared_ptr<Node>> old2new;
    NFA nfa;
    nfa.startState = std::make_shared<Node>();
    old2new[enfa.start_state()] = nfa.startState;
    for (const Node* state : validStates)
    {
        if (old2new.find(state) == old2new.end())
        {
            old2new[state] = std::make_shared<Node>(state->stateName);
        }
        auto eClosure = find_epsilon_closure(state);
        for (auto node : eClosure)
        {
            for (const auto& edge : node->strongEdges)
            {
                if (edge.accept != '\0')
                {
                    auto next = edge.next.get();
                    if (old2new.find(next) == old2new.end())
                    {
                        old2new[next] = std::make_shared<Node>(next->stateName);
                    }
                    old2new[state]->strongEdges.push_back(StrongEdge{edge.accept, old2new[next]});
                }
            }
            for (const auto& edge : node->weakEdges)
            {
                if (edge.accept != '\0')
                {
                    auto next = edge.next.lock().get();
                    if (old2new.find(next) == old2new.end())
                    {
                        old2new[next] = std::make_shared<Node>(next->stateName);
                    }
                    old2new[state]->weakEdges.push_back(WeakEdge{edge.accept, old2new[next]});
                }
            }
        }
        
    }

    return nfa;
}

vector<string> NFA::match_all(const string & text)
{
    using container::distinct_queue;
    using state_pair = std::pair<Node*, string::const_iterator>;

    struct state_pair_equal
    {
        bool operator()(const state_pair& lhs, const state_pair& rhs) const
        {
            return lhs.first == rhs.first;
        }
    };

    vector<string> results;
    distinct_queue<state_pair, state_pair_equal> currentStates;

    currentStates.push({startState.get(), text.begin()});

    while (currentStates.size())
    {
        const auto& current = currentStates.front();
        if (current.second == text.end())
        {
            if (current.first->stateName != "")
            {
                results.push_back(current.first->stateName);
            }
        }
        else
        {
            for (const auto& edge : current.first->strongEdges)
            {
                if (edge.accept == *(current.second))
                {
                    currentStates.push({edge.next.get(), current.second + 1});
                }
            }
            for (const auto& edge : current.first->weakEdges)
            {
                if (edge.accept == *(current.second))
                {
                    currentStates.push({edge.next.lock().get(), current.second + 1});
                }
            }
        }
        currentStates.pop();
    }
    return results;
}

}
}