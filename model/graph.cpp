#include "graph.h"


Graph::Graph(const Graph &other)
    : nextNodeId(other.nextNodeId), nextEdgeId(other.nextEdgeId)
{
    for (const auto& [id, node] : other.nodes)
    {
        if (node)
            nodes[id] = make_unique<Node>(*node);
    }

    for (const auto& [id, edge] : other.edges)
    {
        if (edge)
            edges[id] = make_unique<Edge>(*edge);
    }
}

Graph& Graph::operator=(const Graph& other)
{
    if (this != &other)
    {
        nodes.clear();
        edges.clear();

        nextNodeId = other.nextNodeId;
        nextEdgeId = other.nextEdgeId;

        for (const auto& [id, node] : other.nodes)
        {
            if (node)
                nodes[id] = make_unique<Node>(*node);
        }

        for (const auto& [id, edge] : other.edges)
        {
            if (edge)
                edges[id] = make_unique<Edge>(*edge);
        }
    }
    return *this;
}

//Добавить узел
void Graph::addNode(NodeData data, QPointF position)
{
    int id = nextNodeId++;
    data.name = QString("Node %1").arg(id);
    nodes[id] = make_unique<Node>(id, data, position);
}

//Добавить готовый узел(только для загрузки)
void Graph::addNode(Node node)
{
    int id = node.getId();
    nextNodeId = (id > nextNodeId) ? id+1 : nextNodeId;
    nodes[node.getId()] = make_unique<Node>(node);
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
        if (edge->getSourceId() == nodeId or edge->getTargetId() == nodeId)
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
            if ((edge->getSourceId() == sourceId && edge->getTargetId() == targetId)) //можно в обе стороны связь
                //or (edge->getSourceId() == targetId && edge->getTargetId() == sourceId)) //только в одну сторону связь
                return;
        }

        int id = nextEdgeId++;
        edges[id] = make_unique<Edge>(data, id, sourceId, targetId);
    }
}

//Добавить готовое ребро(только для загрузки)
void Graph::addEdge(Edge edge)
{
    int id = edge.getId();
    nextEdgeId = (id > nextEdgeId) ? id+1 : nextEdgeId;
    edges[edge.getId()] = make_unique<Edge>(edge);
}


//Удалить ребро
bool Graph::removeEdge(int edgeId)
{
    return edges.erase(edgeId) > 0;
}

//Получить конкретный узел
Node* Graph::getNode(int id)
{
    auto it = nodes.find(id);
    return (it != nodes.end()) ? it->second.get() : nullptr;
}

//Получить конкретное ребро
Edge* Graph::getEdge(int id)
{
    auto it = edges.find(id);
    return (it != edges.end()) ? it->second.get() : nullptr;
}

// Очистить граф
void Graph::clear()
{
    nextNodeId = 0;
    nextEdgeId = 0;
    nodes.clear();
    edges.clear();
}

QDataStream& operator<<(QDataStream& out, const Graph& graph)
{
    const auto& nodes = graph.getNodes();
    int nodesCount = nodes.size();
    out << nodesCount;
    for (const auto& [id, node] : nodes)
        out << *node;

    const auto& edges = graph.getEdges();
    int edgesCount = edges.size();
    out << edgesCount;
    for (const auto& [id, edge] : edges)
        out << *edge;
    return out;
}

QDataStream& operator>>(QDataStream& in, Graph& graph)
{
    int nodeCount;
    Node node;
    in >> nodeCount;
    for(int i = 0; i < nodeCount; i++)
    {
        in >> node;
        graph.addNode(node);
    }

    //Ребра
    int edgeCount;
    Edge edge;
    in >> edgeCount;
    for(int i = 0; i < edgeCount; i++)
    {
        in >> edge;
        graph.addEdge(edge);
    }
    return in;
}
