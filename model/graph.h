#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <memory>
#include "edge.h"
#include "node.h"

using namespace std;

class Graph
{
private:
    unordered_map<int, unique_ptr<Node>> nodes; // Словарь узлов: [ID → указатель на узел]
    unordered_map<int, unique_ptr<Edge>> edges; // Словарь рёбер: [ID → указатель на ребро]
    int nextNodeId = 0; //id следующего создаваемого узла
    int nextEdgeId = 0; //id следующего создаваемого ребра

public:
    void addNode(NodeData data, QPointF position);           //Добавить узел
    bool removeNode(int nodeId);                             //Удалить узел
    void addEdge(EdgeData data, int sourceId, int targetId); //Добавить ребро
    bool removeEdge(int edgeId);                             //Удалить ребро
    const auto& getNodes() const { return nodes; } // Получить узлы
    const auto& getEdges() const { return edges; } // Получить ребра
};

#endif // GRAPH_H
