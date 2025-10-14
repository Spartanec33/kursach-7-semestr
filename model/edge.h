#ifndef EDGE_H
#define EDGE_H

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

    int GetId() {return id;} //Получить id ребра
    int GetSourceId() {return sourceNodeId;} //Получить id источника
    int GetTargetId() {return targetNodeId;} //Получить id цели
private:
    const int id;
    int sourceNodeId;
    int targetNodeId;
    EdgeData data;
};

#endif // EDGE_H
