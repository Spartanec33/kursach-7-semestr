#ifndef GRAPHCONTROLLER_H
#define GRAPHCONTROLLER_H

#include <QObject>
#include <QPointF>
#include "graph.h"
#include "graphview.h"

class GraphController : public QObject
{
    Q_OBJECT
public:
    GraphController(Graph* model, GraphView* view, QObject* parent = nullptr);

public slots:
    void addNodeAt(const QPointF& position);
    void addEdge(int fromId, int toId);
    void removeSelectedNode();
    void removeSelectedEdge();

private:
    void removeNode(int id);
    void removeEdge(int id);
    Graph* graph;
    GraphView* view;
};
#endif // GRAPHCONTROLLER_H
