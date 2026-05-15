#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <memory>
#include <queue>
#include <QMessageBox>
#include "edge.h"
#include "node.h"

using namespace std;

class Graph
{
public:
    Graph()= default;
    Graph(const Graph& other);
    void addNode(NodeData data, QPointF position);           //Добавить узел
    void addNode(Node node);                                 //Добавить готовый узел(только для загрузки)
    bool removeNode(int nodeId);                             //Удалить узел

    bool addEdge(EdgeData data, int sourceId, int targetId); //Добавить ребро
    bool addEdge(Edge edge);                                 //Добавить готовое ребро(только для загрузки)
    bool removeEdge(int edgeId);                             //Удалить ребро

    void clear(); // Очистить граф

    const auto& getNodes() const { return nodes; } // Получить все узлы для чтения
    const auto& getEdges() const { return edges; } // Получить все ребра для чтения
    Node* getNode(int id); //Получить конкретный узел
    Edge* getEdge(int id); //Получить конкретное ребро
    bool hasAnyCycle() const; //Проверить на циклы граф
    bool isNameExists(const QString& name, int excludeId) const; //Проверить наличие такого имени у узлов
    // Возвращает пару: {вектор пути, текстовый отчет}
    pair<vector<int>, QString> findShortestPathWithLog(int startId, int endId, int& dist, QString& exitPath) const; //Решить задачу поиска кратчайшего пути методом ДП

    Graph& operator=(const Graph& other);
    friend QTextStream& operator<<(QTextStream& out, const Graph& graph);
    friend QTextStream& operator>>(QTextStream& in, Graph& graph);
private:
    bool canConnect(int source, int target);
    bool dfsCheckCycle(int node, unordered_map<int, bool>& visited,
                           unordered_map<int, bool>& recursionStack) const; //Проверить на цикл ветвь графа

    unordered_map<int, unique_ptr<Node>> nodes; // Словарь узлов: [ID → указатель на узел]
    unordered_map<int, unique_ptr<Edge>> edges; // Словарь рёбер: [ID → указатель на ребро]
    int nextNodeId = 0; //id следующего создаваемого узла
    int nextEdgeId = 0; //id следующего создаваемого ребра
};

#endif // GRAPH_H
