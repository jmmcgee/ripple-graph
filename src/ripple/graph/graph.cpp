// graph.cpp
#include <ripple/graph/graph.h>

#include <iostream>
#include <vector>

#include <cstdint>

int main(int argc, char* argv[])
{
    using Vertex = ripple::Vertex;
    //using VertexOracle = ripple::Oracle<Vertex, std::uint32_t>;
    using Edge = ripple::Edge<Vertex>;
    using Graph = ripple::Graph<Vertex, Edge>;

    //VertexOracle vo;

    size_t const N = 32;
    std::vector<Vertex> vs;

    for (uint32_t i = 0; i < 32; i++)
    {
        vs.push_back(Vertex{i});
    }

    for (auto v : vs)
        std::cout << "Vertex " << v.id_ << std::endl;
}


