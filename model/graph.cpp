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

//Проверить наличие такого имени у узлов
bool Graph::isNameExists(const QString& name, int excludeId) const
{
    for (auto const& [id, node] : nodes)
    {
        if (id != excludeId and node->getData().name == name)
            return true;
    }
    return false;
}

//Добавить узел
void Graph::addNode(NodeData data, QPointF position)
{
    int id = nextNodeId++;

    // Ищем первое свободное имя в формате "Node N"
    QString potentialName;
    int nameIndex = id;
    do
    {
        potentialName = QString("Node %1").arg(nameIndex++);
    }
    while (isNameExists(potentialName, id));

    data.name = potentialName;
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

// Проверка на существующее ребро
bool Graph::canConnect(int source, int target)
{
    if (source == target) return false; // Петли запрещены

    auto sourceNode = nodes.find(source);
    auto targetNode = nodes.find(target);
    if (sourceNode == nodes.end() or targetNode == nodes.end()) // не нашли какой-то узел
        return false;

    for (const auto& [id, edge] : edges)
    {
        // Если уже есть связь A -> B или B -> A
        if ((edge->getSourceId() == source and edge->getTargetId() == target) or //можно в обе стороны связь
            (edge->getSourceId() == target and edge->getTargetId() == source)) //только в одну сторону связь
        {
            return false;
        }
    }
    return true;
}

//Добавить ребро
bool Graph::addEdge(EdgeData data, int sourceId, int targetId)
{
    if(!canConnect(sourceId, targetId))
        return false;
    int id = nextEdgeId++;
    edges[id] = make_unique<Edge>(data, id, sourceId, targetId);

    return true;
}

//Добавить готовое ребро(только для загрузки)
bool Graph::addEdge(Edge edge)
{
    if(!canConnect(edge.getSourceId(), edge.getTargetId()))
        return false;

    if (edges.find(edge.getId()) != edges.end())
            return false;

    int id = edge.getId();
    nextEdgeId = (id > nextEdgeId) ? id+1 : nextEdgeId;
    edges[edge.getId()] = make_unique<Edge>(edge);
    return true;
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

QTextStream& operator<<(QTextStream& out, const Graph& graph)
{
    // Узлы
    const auto& nodes = graph.getNodes();
    int nodesCount = nodes.size();
    out << nodesCount << "\n";
    for (const auto& [id, node] : nodes)
    {
        out << *node << "\n";
    }

    // Ребра
    const auto& edges = graph.getEdges();
    int edgesCount = edges.size();
    out << edgesCount << "\n";
    for (const auto& [id, edge] : edges)
    {
        out << *edge << "\n";
    }
    return out;
}

QTextStream& operator>>(QTextStream& in, Graph& graph)
{
    graph.clear();

    // 1. Читаем узлы
    int nodeCount;
    in >> nodeCount;
    if (in.status() != QTextStream::Ok) return in;
    for(int i = 0; i < nodeCount; i++)
    {
        Node node;
        in >> node;
        if (in.status() != QTextStream::Ok) return in;
        //Проверка: дубликат имени
        if(graph.isNameExists(node.getData().name, node.getId()))
        {
            in.setStatus(QTextStream::ReadCorruptData);
            return in;
        }
        graph.addNode(node);
    }

    // 2. Читаем ребра
    int edgeCount;
    in >> edgeCount;
    if (in.status() != QTextStream::Ok) return in;
    for(int i = 0; i < edgeCount; i++)
    {
        Edge edge;
        in >> edge;
        if (in.status() != QTextStream::Ok) return in;

        // ПРОВЕРКА 1: Отрицательный вес
        if (edge.getData().weight < 0)
        {
            in.setStatus(QTextStream::ReadCorruptData);
            return in;
        }

        // ПРОВЕРКА 2: Некорректные ребра
        if (!graph.addEdge(edge))
        {
            in.setStatus(QTextStream::ReadCorruptData);
            return in;
        }
    }
    return in;
}

//Проверить на циклы граф
bool Graph::hasAnyCycle() const
{
    unordered_map<int, bool> visited;
    unordered_map<int, bool> recursionStack;

    // Инициализируем карты для всех существующих узлов
    for (auto const& [id, node] : nodes)
    {
        visited[id] = false;
        recursionStack[id] = false;
    }

    // Запускаем DFS для каждого еще не посещенного узла
    for (auto const& [id, node] : nodes)
    {
        if (!visited[id])
        {
            if (dfsCheckCycle(id, visited, recursionStack))
                return true; // Цикл найден
        }
    }
    return false;
}

//Проверить на цикл ветвь графа
bool Graph::dfsCheckCycle(int nodeId, unordered_map<int, bool>& visited,
                          unordered_map<int, bool>& recursionStack) const
{
    visited[nodeId] = true;
    recursionStack[nodeId] = true;

    // Проходим по всем исходящим ребрам
    for (auto const& [edgeId, edge] : edges)
    {
        if (edge->getSourceId() == nodeId)
        {
            int neighborId = edge->getTargetId();

            // Если сосед еще не посещен, идем в него рекурсивно
            if (!visited[neighborId])
            {
                if (dfsCheckCycle(neighborId, visited, recursionStack))
                {
                    return true;
                }
            }

            // Если сосед уже в стеке рекурсии — мы нашли цикл!
            else if (recursionStack[neighborId])
            {
                return true;
            }
        }
    }

    // Убираем узел из стека рекурсии при возврате
    recursionStack[nodeId] = false;
    return false;
}

//Решить задачу поиска кратчайшего пути методом ДП
pair<vector<int>, QString> Graph::findShortestPathWithLog(int startId, int endId, int& exitDist, QString& exitPath) const
{
    QString log;
    log += QString("==================================================\n");
    log += QString("   ЗАПУСК АЛГОРИТМА ПОИСКА КРАТЧАЙШЕГО ПУТИ (ДП)  \n");
    log += QString("==================================================\n");
    log += QString("Стартовый узел: %1\n").arg(nodes.at(startId)->getData().name);
    log += QString("Целевой узел:   %2\n").arg(nodes.at(endId)->getData().name);
    log += QString("--------------------------------------------------\n\n");

    unordered_map<int, double> distances;
    unordered_map<int, int> predecessors;
    for (auto const& [id, node] : nodes)
    {
        distances[id] = numeric_limits<double>::infinity();
        predecessors[id] = -1;
    }
    distances[startId] = 0;

    using pii = pair<double, int>;
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, startId});

    int step = 1;

    while (!pq.empty())
    {
        int u = pq.top().second;
        double d = pq.top().first;
        pq.pop();

        // Пропускаем устаревшие пары в очереди (важная оптимизация Дейкстры/ДП)
        if (d > distances[u]) continue;

        log += QString("Шаг %1. Обработка узла [%2] (Текущее мин. расстояние: %3)\n")
               .arg(step++).arg(nodes.at(u)->getData().name).arg(d);
        log += QString("  Проверка исходящих ребер:\n");

        bool hasEdges = false;
        for (auto const& [edgeId, edge] : edges)
        {
            if (edge->getSourceId() == u)
            {
                hasEdges = true;
                int v = edge->getTargetId();
                double weight = edge->getData().weight;

                double newDist = distances[u] + weight;
                log += QString("    -> к [%1] (вес %2): ").arg(nodes.at(v)->getData().name).arg(weight);

                if (newDist < distances[v])
                {
                    log += QString("УСПЕХ! Путь улучшен: %1 -> %2\n")
                           .arg(distances[v] == numeric_limits<double>::infinity() ? "INF" : QString::number(distances[v]))
                           .arg(newDist);
                    distances[v] = newDist;
                    predecessors[v] = u;
                    pq.push({distances[v], v});
                }
                else
                {
                    log += QString("Мимо (старое расстояние %1 <= нового %2)\n")
                           .arg(distances[v]).arg(newDist);
                }
            }
        }
        if (!hasEdges) {
            log += QString("    (Нет исходящих ребер)\n");
        }
        log += QString("\n"); // Разделитель между шагами
    }

    // Восстановление пути
    vector<int> path;
    log += QString("==================================================\n");
    log += QString("                 РЕЗУЛЬТАТ ПОИСКА                 \n");
    log += QString("==================================================\n");

    if (predecessors[endId] != -1 || startId == endId)
    {
        for (int v = endId; v != -1; v = predecessors[v])
        {
            path.push_back(v);
            if (v == startId) break;
        }
        reverse(path.begin(), path.end());

        log += "Итоговый маршрут: ";
        exitPath = "";
        for(int i = 0; i < path.size(); ++i)
        {
            QString nodeName = nodes.at(path[i])->getData().name;
            log += nodeName + (i == path.size()-1 ? "" : " -> ");
            exitPath += nodeName + (i == path.size()-1 ? "" : " -> ");
        }

        log += QString("\nПолная стоимость пути: %1\n").arg(distances[endId]);
        exitDist = distances[endId];
    }
    else
    {
        log += "СТАТУС: Путь не существует!\n";
        exitDist = -1;
        exitPath = "Путь не найден";
    }
    log += QString("==================================================\n");

    return {path, log};
}
