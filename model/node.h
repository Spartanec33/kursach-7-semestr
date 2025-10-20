#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QPointF>

// Данные узла - вся информация о узле
struct NodeData
{
    QString name; // Название узла
};

class Node
{
public:
    Node(int nodeId, NodeData d, QPointF p) : id(nodeId), data(d), position(p) {}

    int getId() { return id; }                 // Получить Id
    const NodeData& getData() { return data; } // Получить данные
    void setData(NodeData d) { data = d; }     // Установить данные
    QPointF getPosition() { return position; }        // Получить позицию
    void setPosition(QPointF pos) { position = pos; } // Установить позицию

private:
    NodeData data;    //Данные узла
    QPointF position; // Позиция узла
    const int id;     //Id узла
};

#endif // NODE_H
