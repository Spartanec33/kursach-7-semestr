#include "graphcontroller.h"

GraphController::GraphController(Graph* model, GraphView* v, QObject* parent)
    : QObject(parent), graph(model), view(v)
{
    view->setGraph(graph);
    connect(view, &GraphView::clicked, this, &GraphController::addNodeAt);
    connect(view, &GraphView::edgeCreated, this, &GraphController::addEdge);
}

//Добавить узел в точку
void GraphController::addNodeAt(const QPointF& position)
{
    NodeData data;
    data.position = position;
    graph->addNode(data);
    view->drawGraph();
}

//Добавить ребро между узлами
void GraphController::addEdge(int fromId, int toId)
{
    EdgeData data;
    data.info = QString("Edge %1->%2").arg(fromId).arg(toId);
    graph->addEdge(data, fromId, toId);
    view->drawGraph();
}

// Удалить узел по его id
void GraphController::removeNode(int id)
{
    if (graph->removeNode(id))
        view->drawGraph();
}

// Удалить ребро по его id
void GraphController::removeEdge(int id)
{
    if (graph->removeEdge(id))
        view->drawGraph();
}

// Удалить выбранный узел
void GraphController::removeSelectedNode()
{
    removeNode(view->getSelectedNode());
    view->deselectNode();
}

// Удалить выбранное ребро
void GraphController::removeSelectedEdge()
{
    //removeEdge(view->getSelectedEdge());
    //view->deselectEdge();
}



