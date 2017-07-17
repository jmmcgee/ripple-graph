// graph.h
#ifndef RIPPLE_GRAPH_H_INCLUDED
#define RIPPLE_GRAPH_H_INCLUDED

#include <ripple/basics/tagged_integer.h>

#include <boost/bimap/bimap.hpp>
#include <boost/optional.hpp>

#include <cstdint>
#include <unordered_map>
#include <vector>

namespace ripple {
using VertexT = template<typename ID = std::uint32_t, typename State>;

template<typename ID = std::uint32_t, typename State>
class Vertex
{
    friend class Manager<Vertex>;
    ID id_;
    State state_;
};


template<template <typename ID = std::uint32_t, typename State> class VertexT>
class Manager
{
    std::unordered_map<VertexT::ID, VertexT::State> peers_;

public:
    Manager();

    ID nextID() const;

    ID create(ID id = nextID());

    bool insert(ID id, State state);

    std::optional<VertexT> lookup(std::uint32_t id);

    std::size_t numPeers();
};

template<typename ID>
struct Edge
{
    ID from, to;
};

template <typename V, typename E>
struct Graph
{
    //Oracle<V>* vertexOracle_;
    std::unordered_map<V, std::unordered_map<V, E>> adjacencyMatrix_;

    /*Graph(Oracle<V>* vertexOracle, std::vector<E> edges)
        : vertexOracle_{vertexOracle}
    {
    }*/
};

} //ripple
#endif
