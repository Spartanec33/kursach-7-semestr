#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QPointF>
#include <QTextStream>
#include <QList>

// Данные узла - вся информация о узле
// При изменении обновить: потоки вывода, showNodeSelected, NodeForm, processNodeForm
struct NodeData
{
    QString name;            // Название узла

    friend QTextStream& operator<<(QTextStream& out, const NodeData& data);
    friend QTextStream& operator>>(QTextStream& in, NodeData& data);
};

//Узел
class Node
{
public:
    Node()= default;
    Node(const Node& other);
    Node(int nodeId, NodeData d, QPointF p) : id(nodeId), data(d), position(p) {}

    int getId() { return id; }                 // Получить Id
    const NodeData& getData() { return data; } // Получить данные
    void setData(NodeData d) { data = d; }     // Установить данные
    QPointF getPosition() { return position; }        // Получить позицию
    void setPosition(QPointF pos) { position = pos; } // Установить позицию

    friend QTextStream& operator<<(QTextStream& out, const Node& node);
    friend QTextStream& operator>>(QTextStream& in, Node& node);
private:
    NodeData data;    //Данные узла
    QPointF position; // Позиция узла
    int id;           //Id узла
};

#endif // NODE_H
