#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <memory>
#include "edge.h"
#include "node.h"

using namespace std;

class Graph
{
public:
    void addNode(NodeData data, QPointF position);           //Добавить узел
    void addNode(Node node);                                 //Добавить готовый узел(только для загрузки)
    bool removeNode(int nodeId);                             //Удалить узел

    void addEdge(EdgeData data, int sourceId, int targetId); //Добавить ребро
    void addEdge(Edge edge);                                 //Добавить готовое ребро(только для загрузки)
    bool removeEdge(int edgeId);                             //Удалить ребро

    void clear(); // Очистить граф

    const auto& getNodes() const { return nodes; } // Получить все узлы для чтения
    const auto& getEdges() const { return edges; } // Получить все ребра для чтения
    Node* getNode(int id); //Получить конкретный узел
    Edge* getEdge(int id); //Получить конкретное ребро

private:
    unordered_map<int, unique_ptr<Node>> nodes; // Словарь узлов: [ID → указатель на узел]
    unordered_map<int, unique_ptr<Edge>> edges; // Словарь рёбер: [ID → указатель на ребро]
    int nextNodeId = 0; //id следующего создаваемого узла
    int nextEdgeId = 0; //id следующего создаваемого ребра
};

#endif // GRAPH_H
