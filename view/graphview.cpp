#include "graphview.h"
#include <QMouseEvent>
#include "styleSheet.h"

GraphView::GraphView(QWidget* parent)
    : QGraphicsView(parent), scene(new QGraphicsScene(this))
{
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
    setSceneRect(0, 0, 800, 600);
}

// Выбрать модель графа
void GraphView::setGraph(const Graph* g)
{
    graph = g;
    drawGraph();
}

//Отрисовать граф
void GraphView::drawGraph()
{
    scene->clear();

    if (!graph) return;

    // Отрисовать ребра
    const auto& edges = graph->getEdges();
    for (const auto& [id, edge] : edges)
    {
        auto sourceNode = graph->getNodes().at(edge->GetSourceId()).get();
        auto targetNode = graph->getNodes().at(edge->GetTargetId()).get();

        QPointF sourcePos = sourceNode->GetData().position;
        QPointF targetPos = targetNode->GetData().position;
        scene->addLine(QLineF(sourcePos, targetPos), QPen(edgeColor, edgeFat));
    }

    // Отрисовать узлы
    const auto& nodes = graph->getNodes();
    for (const auto& [id, node] : nodes)
    {
        QPointF center = node->GetData().position;

        // Цвет в зависимости от выделенности
        QBrush brush = (id == selectedNodeId) ? QBrush(selectedNodeColor) : QBrush(nodeColor);

        auto* ellipse = scene->addEllipse(center.x() - nodeSize, center.y() - nodeSize, nodeSize*2, nodeSize*2,
                                         QPen(borderColor, borderFat), brush);

        // Отрисовать текст в узле
        auto* text = scene->addText(node->GetData().name);
        QRectF textRect = text->boundingRect();
        text->setPos(center.x() - textRect.width()/2,
                     center.y() - textRect.height()/2);
        text->setDefaultTextColor(nodeTextColor);
    }
}

//Поиск узла в точке
int GraphView::findNodeAt(QPointF position)
{
    if (!graph) return -1;

    const auto& nodes = graph->getNodes();
    for (const auto& [id, node] : nodes)
    {
        QPointF nodePos = node->GetData().position;
        if (nodePos.isNull()) continue;

        // Проверяем расстояние до центра узла
        if (QLineF(nodePos, position).length() < nodeSize)
            return id;
    }
    return -1;
}

//Нажатие мышью
void GraphView::mousePressEvent(QMouseEvent* event)
{
    QPointF scenePos = mapToScene(event->pos());
    int clickedNodeId = findNodeAt(scenePos);

    if (event->button() == Qt::LeftButton)
    {
        if (clickedNodeId == -1)
        {
            if (selectedNodeId != -1)
                deselectNode();
            else
                emit clicked(scenePos);
        }
        else
        {
            selectedNodeId = clickedNodeId;
            draggedNodeId = clickedNodeId;  // начинаем перетаскивание
            emit nodeSelected(clickedNodeId);
        }
    }
    else if (event->button() == Qt::RightButton && clickedNodeId != -1)
    {
        // ПКМ на узле - создаём связь
        if (selectedNodeId == -1)
        {
            // Первый узел для связи
            selectedNodeId = clickedNodeId;
            emit nodeSelected(clickedNodeId);
        }
        else if (selectedNodeId != clickedNodeId)
        {
            // Второй узел - создаём связь
            emit edgeCreated(selectedNodeId, clickedNodeId);
            deselectNode();
        }
    }
    drawGraph();
}

//Движение мышью
void GraphView::mouseMoveEvent(QMouseEvent* event)
{
    if (draggedNodeId != -1)
    {
        QPointF scenePos = mapToScene(event->pos());
        auto node = graph->getNodes().at(draggedNodeId).get();
        node->SetPosition(scenePos);
        drawGraph();
    }
}

//Отпустили кнопку мыши
void GraphView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && draggedNodeId != -1)
        draggedNodeId = -1;
}
