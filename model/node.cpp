#include "node.h"

Node::Node(const Node &other)
    : id(other.id)
    , data(other.data)
    , position(other.position)
{}


QDataStream& operator<<(QDataStream& out, const NodeData& data)
{
    out << data.name;
    return out;
}

QDataStream& operator>>(QDataStream& in, NodeData& data)
{
    in >> data.name;
    return in;
}

QDataStream& operator<<(QDataStream& out, const Node& node)
{
    out << node.id << node.data << node.position;
    return out;
}

QDataStream& operator>>(QDataStream& in, Node& node)
{
    in >> node.id >> node.data >> node.position;
    return in;
}

