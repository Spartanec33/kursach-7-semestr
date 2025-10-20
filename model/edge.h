#ifndef EDGE_H
#define EDGE_H

#include <QLineF>
#include <QString>

// Данные ребра - хранит информацию о связи
struct EdgeData
{
    QString info;
};

// Класс ребра - представляет связь между двумя узлами
class Edge
{
public:
    Edge(EdgeData data, int id, int source, int target) : data(data), id(id), sourceNodeId(source), targetNodeId(target) {}

    int getId() {return id;} //Получить id ребра
    int getSourceId() {return sourceNodeId;} //Получить id источника
    int getTargetId() {return targetNodeId;} //Получить id цели
    QLineF getLine() { return line; } // Получить линию
    void setLine(QLineF l) { line = l; } // Установить линию

private:
    const int id;     //id ребра
    int sourceNodeId; //id начального узла
    int targetNodeId; //id конечного узла
    QLineF line;      //Линия, представляющая ребро
    EdgeData data;    //Данные ребра
};

#endif // EDGE_H
