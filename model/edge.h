#ifndef EDGE_H
#define EDGE_H

#include <QLineF>
#include <QString>
#include <QDataStream>
#include <QList>

// Данные ребра - хранит информацию о связи
// При изменении обновить: потоки вывода, showEdgeSelected, EdgeForm, processEdgeForm
struct EdgeData
{
    QString info;            // Описание поставки
    QList<QString> products; // Изделия, которые передаются в поставке

    friend QDataStream& operator<<(QDataStream& out, const EdgeData& data);//Сериализация
    friend QDataStream& operator>>(QDataStream& in, EdgeData& data);       //Десериализация
};

// Класс ребра - представляет связь между двумя узлами
class Edge
{
public:
    Edge()= default;
    Edge(const Edge& other);
    Edge(EdgeData data, int id, int source, int target) : data(data), id(id), sourceNodeId(source), targetNodeId(target) {}

    int getId() {return id;} //Получить id ребра
    int getSourceId() {return sourceNodeId;} //Получить id источника
    int getTargetId() {return targetNodeId;} //Получить id цели
    QLineF getLine() { return line; }    // Получить линию
    void setLine(QLineF l) { line = l; } // Установить линию
    EdgeData getData() {return data;}   //Получить данные
    void setData(EdgeData d){data = d;} //Установить данные

    friend QDataStream& operator<<(QDataStream& out, const Edge& edge);//Сериализация
    friend QDataStream& operator>>(QDataStream& in, Edge& edge);       //Десериализация

private:
    int id;           //id ребра
    int sourceNodeId; //id начального узла
    int targetNodeId; //id конечного узла
    QLineF line;      //Линия, представляющая ребро
    EdgeData data;    //Данные ребра
};

#endif // EDGE_H
