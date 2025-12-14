#include "edge.h"

Edge::Edge(const Edge &other)
    : id(other.id)
    , sourceNodeId(other.sourceNodeId)
    , targetNodeId(other.targetNodeId)
    , line(other.line)
    , data(other.data)
{}


QDataStream& operator<<(QDataStream& out, const EdgeData& data)
{
    out << data.info << data.products;
    return out;
}

QDataStream& operator>>(QDataStream& in, EdgeData& data)
{
    in >> data.info >> data.products;
    return in;
}

QDataStream& operator<<(QDataStream& out, const Edge& edge)
{
    out << edge.id
        << edge.sourceNodeId
        << edge.targetNodeId
        << edge.data
        << edge.line;
    return out;
}

QDataStream& operator>>(QDataStream& in, Edge& edge)
{
    in  >> edge.id
        >> edge.sourceNodeId
        >> edge.targetNodeId
        >> edge.data
        >> edge.line;
    return in;
}

