#ifndef GRAPHCONTROLLER_H
#define GRAPHCONTROLLER_H

#include <QFile>
#include <QObject>
#include <QPointF>
#include "graph.h"
#include "graphview.h"
#include "nodeform.h"
#include "edgeform.h"
#include <QMessageBox>
#include <vector>

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

    void clearGraph();   //Очистить граф
    void showInfoForm(); //Показать окно информации

    void saveGraph(); //Сохранить граф
    void loadGraph(); //Загрузить граф

    void graphChanged(); //Обработать изменение графа

    void undo();   //Шаг в прошлое в истории графов
    void redo();   //Шаг в будущее в истории графов

    bool handleCloseEvent();
private:
    void removeNode(int id); // Удалить узел по его id
    void removeEdge(int id); // Удалить ребро по его id

    void processNodeForm(int selectedNodeId); // Обработка информационного окошка узла
    void processEdgeForm(int selectedEdgeId); // Обработка информационного окошка ребра

    vector<Graph> graphHistory; // История графов
    int currentIndex = -1;      // Текущее положение в истории графов
    Graph* graph;    //Модель
    GraphView* view; //Вид

};
#endif // GRAPHCONTROLLER_H
