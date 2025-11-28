#include "node.h"

Node::Node(const Node &other)
    : id(other.id)
    , data(other.data)
    , position(other.position)
{}


QDataStream& operator<<(QDataStream& out, const NodeData& data)
{
    out << data.name << data.info;
    return out;
}

QDataStream& operator>>(QDataStream& in, NodeData& data)
{
    in >> data.name >> data.info;
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

