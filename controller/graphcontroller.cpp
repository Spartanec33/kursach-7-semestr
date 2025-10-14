#include "graphcontroller.h"

GraphController::GraphController(Graph* model, GraphView* v, QObject* parent)
    : QObject(parent), graph(model), view(v)
{
    view->setGraph(graph);
    connect(view, &GraphView::clicked, this, &GraphController::addNodeAt);
    connect(view, &GraphView::edgeCreated, this, &GraphController::addEdge);
}

void GraphController::addEdge(int fromId, int toId)
{
    EdgeData data;
    data.info = QString("Edge %1->%2").arg(fromId).arg(toId);
    graph->addEdge(data, fromId, toId);
    view->drawGraph();
}



void GraphController::removeNode(int id)
{
    if (graph->removeNode(id))
        view->drawGraph();
}

void GraphController::removeEdge(int id)
{
    if (graph->removeEdge(id))
        view->drawGraph();
}

void GraphController::removeSelectedNode()
{
    removeNode(view->getSelectedNode());
    view->deselectNode();
}

void GraphController::removeSelectedEdge()
{

}

void GraphController::addNodeAt(const QPointF& position)
{
    NodeData data;
    data.name = QString("Node %1").arg(graph->getNextNodeId());
    data.position = position;
    graph->addNode(data);
    view->drawGraph();
}

