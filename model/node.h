#ifndef NODE_H
#define NODE_H

#include <QString>
#include <QPointF>
#include <QDataStream>

// Данные узла - вся информация о узле
struct NodeData
{
    QString name; // Название узла
    QString info; // Текстовое описание узла
    friend QDataStream& operator<<(QDataStream& out, const NodeData& data); //Сериализация
    friend QDataStream& operator>>(QDataStream& in, NodeData& data);        //Десериализация
};

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

    friend QDataStream& operator<<(QDataStream& out, const Node& node); //Сериализация
    friend QDataStream& operator>>(QDataStream& in, Node& node);        //Десериализация
private:
    NodeData data;    //Данные узла
    QPointF position; // Позиция узла
    int id;           //Id узла
};

#endif // NODE_H
