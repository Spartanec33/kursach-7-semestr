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
    void addNodeAt(const QPointF& position); // Добавить узел в точку
    void addEdge(int fromId, int toId);      // Добавить ребро между узлами
    void removeSelectedNode(); // Удалить выбранный узел
    void removeSelectedEdge(); // Удалить выбранное ребро

private:
    void removeNode(int id); // Удалить узел по его id
    void removeEdge(int id); // Удалить ребро по его id
    Graph* graph; //модель
    GraphView* view; //вид
};
#endif // GRAPHCONTROLLER_H
