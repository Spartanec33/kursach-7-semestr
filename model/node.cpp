#include "node.h"

Node::Node(const Node &other)
    : id(other.id)
    , data(other.data)
    , position(other.position)
{}


QTextStream& operator<<(QTextStream& out, const NodeData& data)
{
    out << data.name.trimmed().replace(" ", "_");
    return out;
}

QTextStream& operator>>(QTextStream& in, NodeData& data)
{
    QString rawName;
    in >> rawName;
    data.name = rawName.replace("_", " ");
    return in;
}

QTextStream& operator<<(QTextStream& out, const Node& node)
{
    // Формат: id x y name
    out << node.id << " "
        << node.position.x() << " "
        << node.position.y() << " "
        << node.data;
    return out;
}

QTextStream& operator>>(QTextStream& in, Node& node)
{
    double x, y;
    in >> node.id >> x >> y >> node.data;
    node.position = QPointF(x, y);
    return in;
}

