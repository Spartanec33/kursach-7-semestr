#include "edge.h"

Edge::Edge(const Edge &other)
    : id(other.id)
    , sourceNodeId(other.sourceNodeId)
    , targetNodeId(other.targetNodeId)
    , line(other.line)
    , data(other.data)
{}


QTextStream& operator<<(QTextStream& out, const EdgeData& data)
{
    out << data.weight;
    return out;
}

QTextStream& operator>>(QTextStream& in, EdgeData& data)
{
    in >> data.weight;
    return in;
}

QTextStream& operator<<(QTextStream& out, const Edge& edge)
{
    // Формат: id sourceId targetId weight
    out << edge.id << " "
        << edge.sourceNodeId << " "
        << edge.targetNodeId << " "
        << edge.data;
    return out;
}

QTextStream& operator>>(QTextStream& in, Edge& edge)
{
    in >> edge.id >> edge.sourceNodeId >> edge.targetNodeId >> edge.data;
    return in;
}
