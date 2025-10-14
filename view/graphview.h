#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "graph.h"
#include "styleSheet.h"

class GraphView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphView(QWidget* parent = nullptr);
    int getSelectedNode() {return selectedNodeId;}
    void setGraph(const Graph* graph); // Выбрать модель графа
    void drawGraph(); //Отрисовать граф
    void deselectNode(){selectedNodeId = -1;} //Снять выделение узла

signals:
    void clicked(QPointF position); //Сигнал клика на поле
    void nodeSelected(int nodeId);  //Сигнал выбора узла
    void edgeCreated(int fromId, int toId); //Сигнал создания ребра

protected:
    void mousePressEvent(QMouseEvent* event) override;//Нажатие мышью
    void mouseMoveEvent(QMouseEvent* event) override;//Движение мышью
    void mouseReleaseEvent(QMouseEvent* event) override;//Отпустили кнопку мыши

private:
    int findNodeAt(QPointF position); //Поиск узла в точке

    //Состояния view
    int draggedNodeId = -1;
    int selectedNodeId = -1;

    const Graph* graph = nullptr;//выбранная модель
    QGraphicsScene* scene;
};

#endif // GRAPHVIEW_H
