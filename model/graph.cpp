#include "graph.h"

//Добавить узел
void Graph::addNode(NodeData data)
{
    int id = nextNodeId++;
    data.name = QString("Node %1").arg(id);
    nodes[id] = make_unique<Node>(id, data);
}

//Удалить узел
bool Graph::removeNode(int nodeId)
{
    if (nodes.find(nodeId) == nodes.end())
        return false;

    //Находим ВСЕ рёбра, связанные с этим узлом
    vector<int> edgesToRemove;
    for (const auto& [id, edge] : edges)
    {
        if (edge->GetSourceId() == nodeId or edge->GetTargetId() == nodeId)
            edgesToRemove.push_back(id);
    }

    //Удаляем найденные рёбра
    for (int edgeId : edgesToRemove)
        edges.erase(edgeId);

    // Шаг 3: Удаляем сам узел
    nodes.erase(nodeId);
    return true;
}

//Добавить ребро
void Graph::addEdge(EdgeData data, int sourceId, int targetId)
{
    if (sourceId == targetId)
        return;

    auto sourceNode = nodes.find(sourceId);
    auto targetNode = nodes.find(targetId);
    if (sourceNode != nodes.end() and targetNode != nodes.end())
    {
        // Проверка на существующее ребро
        for (const auto& [id, edge] : edges)
        {
            if ((edge->GetSourceId() == sourceId && edge->GetTargetId() == targetId)) //можно в обе стороны связь
                //or (edge->GetSourceId() == targetId && edge->GetTargetId() == sourceId)) //только в одну сторону связь
                return;
        }

        int id = nextEdgeId++;
        edges[id] = make_unique<Edge>(data, id, sourceId, targetId);
    }
}

//Удалить ребро
bool Graph::removeEdge(int edgeId)
{
    return edges.erase(edgeId) > 0;
}
