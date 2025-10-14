#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QPointF>

// Данные узла - вся информация о узле
struct NodeData
{
    QString name; // Название узла
    QPointF position; // Позиция узла
};

class Node
{
public:
    Node(int nodeId, NodeData d) : id(nodeId), data(d) {}

    int GetId() { return id; } // Получить Id
    const NodeData& GetData() { return data; } // Получить данные
    void SetData(NodeData d) { data = d; } // Установить данные
    QPointF GetPosition() { return data.position; } // Получить позицию
    void SetPosition(QPointF pos) { data.position = pos; } // Установить позицию

private:
    NodeData data;
    const int id;
};

#endif // NODE_H
