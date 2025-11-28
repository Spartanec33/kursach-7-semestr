#ifndef GRAPHVIEW_H
#define GRAPHVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "graph.h"

class GraphView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphView(QWidget* parent = nullptr);

    int getSelectedNode() {return selectedNodeId;} //Взять id выделенного узла
    int getSelectedEdge() {return selectedEdgeId;} //Взять id выделенного ребра
    void setGraph(const Graph* graph); // Выбрать модель графа
    void drawGraph();                  //Отрисовать граф
    void deselectNode(){selectedNodeId = -1;}                     //Снять выделение узла
    void deselectEdge(){selectedEdgeId = -1;}                     //Снять выделение ребра
    void deselectAll(){selectedNodeId = -1; selectedEdgeId = -1;} //Снять все выделения

signals:
    void clicked(QPointF position);         //Сигнал клика на поле
    void nodeSelected(int nodeId);          //Сигнал выбора узла
    void edgeSelected(int edgeId);          //Сигнал выбора ребра
    void edgeCreated(int fromId, int toId); //Сигнал создания ребра
    void moved();

protected:
    void handleLeftClick(int clickedNodeId, int clickedEdgeId, QPointF scenePos); //Обработка ЛКМ
    void handleRightClick(int clickedNodeId);                                     //Обработка ПКМ
    void mousePressEvent(QMouseEvent* event) override;                            //Обработка нажатия мышью
    void mouseMoveEvent(QMouseEvent* event) override;                             //Обработка движения мышью
    void mouseReleaseEvent(QMouseEvent* event) override;                          //Обработка отпуска кнопки мыши

private:
    QLineF applyEdgeOffset(QPointF start, QPointF end); //Сместить ребро
    void drawEdges();                                   // Отрисовать ребра
    void drawArrow(QLineF line, bool isSelected);       // Отрисовать стрелку
    void drawNodeText(QPointF center, QString name); // Отрисовать текст в узле
    void drawNodes();                                // Отрисовать узлы
    int findNodeAt(QPointF position); // Поиск узла в точке
    int findEdgeAt(QPointF position); // Поиск ребра в точке
    double distanceToLine(QPointF point, QPointF lineStart, QPointF lineEnd); // Найти расстояние от точки до линии

    //Состояния view
    int draggedNodeId = -1;      // Какой узел тянем
    int selectedNodeId = -1;     // Выбранный узел
    int selectedEdgeId = -1;     // Выбранное ребро

    const Graph* graph = nullptr;//выбранная модель
    QGraphicsScene* scene;

};

#endif // GRAPHVIEW_H
