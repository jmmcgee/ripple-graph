#ifndef RIPPLE_PEER_H_INCLUDED
#define RIPPLE_PEER_H_INCLUDED

#include <ripple/graph/graph.h>

#include <cstdint>
#include <unordered_map>

namespace ripple {

class Peer : Vertex
{
public:
    using ID = std::uint32_t;

    struct State
    {
    };

    class Manager
    {
        unordered<Peer> peers_;

    public:
        Manager();

        Peer
        create();

        bool
        insert(ID id, State state);

        std::optional<Peer>
        lookup(std::uint32_t id);

        std::size_t
        numPeers();
    };

private:
    ID id_ const;
    State state_;

public:
    ID
    id()
    {
        return id_;
    }

    State
    state()
    {
        return state_;
    }
};

} // ripple
#endif