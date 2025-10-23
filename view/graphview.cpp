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

//Сместить ребро
QLineF GraphView::applyEdgeOffset(QPointF start, QPointF end)
{
    QLineF line(start, end);

    // Вычисляем перпендикулярный вектор
    QPointF direction = end - start;
    QPointF perpendicular(-direction.y(), direction.x()); // Поворот на 90 градусов

    // Нормализуем перпендикуляр
    double length = sqrt(perpendicular.x() * perpendicular.x() + perpendicular.y() * perpendicular.y());
    if (length > 0)
        perpendicular /= length;

    // Смещаем линию
    line.translate(perpendicular * edgeOffset);

    return line;
}

// Отрисовать стрелку
void GraphView::drawArrow(QLineF line, bool isSelected)
{
    // Вычисляем угол линии
    double angle = atan2(line.dy(), -line.dx());

    // Точка для основания стрелки (посередине линии)
    QPointF arrowBase = line.pointAt(0.5);

    // Создаем точки для стрелки
    QPointF arrowP1 = arrowBase + QPointF(
                sin(angle + M_PI / 3) * arrowSize,
                cos(angle + M_PI / 3) * arrowSize
                );

    QPointF arrowP2 = arrowBase + QPointF(
               sin(angle + M_PI - M_PI / 3) * arrowSize,
                cos(angle + M_PI - M_PI / 3) * arrowSize
                );

    // Создаем полигон для стрелки
    QPolygonF arrowHead;
    arrowHead << arrowBase << arrowP1 << arrowP2;

    QBrush brush = (isSelected) ? QBrush(selectedArrowColor) : QBrush(arrowColor); // Цвет в зависимости от выделенности
    // Добавляем стрелку на сцену
    scene->addPolygon(arrowHead,QPen(edgeColor, edgeFat), brush);
}

// Отрисовать ребра
void GraphView::drawEdges()
{
    if (!graph) return;

    const auto& edges = graph->getEdges();
    for (const auto& [id, edge] : edges)
    {
        auto sourceNode = graph->getNodes().at(edge->getSourceId()).get();
        auto targetNode = graph->getNodes().at(edge->getTargetId()).get();

        QLineF line = applyEdgeOffset(sourceNode->getPosition(),  targetNode->getPosition());
        edge->setLine(line);
        QColor color = (id == selectedEdgeId) ? selectedEdgeColor : edgeColor;
        scene->addLine(line, QPen(color, edgeFat));
        drawArrow(line, id == selectedEdgeId);
    }
}



// Отрисовать текст в узле
void GraphView::drawNodeText(QPointF center, QString name)
{
    auto* text = scene->addText(name);
    QRectF textRect = text->boundingRect();
    text->setPos(center.x() - textRect.width()/2,
                 center.y() - textRect.height()/2);
    text->setDefaultTextColor(nodeTextColor);
    text->setFont(QFont("Arial", textSize));
}

// Отрисовать узлы
void GraphView::drawNodes()
{
    const auto& nodes = graph->getNodes();
    for (const auto& [id, node] : nodes)
    {
        QPointF center = node->getPosition();
        QBrush brush = (id == selectedNodeId) ? QBrush(selectedNodeColor) : QBrush(nodeColor); // Цвет в зависимости от выделенности
        scene->addEllipse(center.x() - nodeSize, center.y() - nodeSize, nodeSize*2, nodeSize*2, QPen(borderColor, borderFat), brush);
        drawNodeText(center, node->getData().name);
    }
}

//Отрисовать граф
void GraphView::drawGraph()
{
    scene->clear();

    if (!graph) return;

    drawEdges();
    drawNodes();
}

//Поиск узла в точке
int GraphView::findNodeAt(QPointF position)
{
    if (!graph) return -1;

    const auto& nodes = graph->getNodes();
    for (const auto& [id, node] : nodes)
    {
        QPointF nodePos = node->getPosition();
        if (nodePos.isNull()) continue;

        // Проверяем расстояние до центра узла
        if (QLineF(nodePos, position).length() < nodeSize)
            return id;
    }
    return -1;
}

// Найти расстояние от точки до линии
double GraphView::distanceToLine(QPointF point, QPointF lineStart, QPointF lineEnd)
{
    QLineF line(lineStart, lineEnd);
    if (line.isNull()) return QLineF(point, lineStart).length();

    // Проекция точки на линию
    double t = ((point.x() - lineStart.x()) * (lineEnd.x() - lineStart.x()) +
                (point.y() - lineStart.y()) * (lineEnd.y() - lineStart.y())) /
               (line.length() * line.length());

    t = std::max(0.0, std::min(1.0, t));

    QPointF closestPoint = lineStart + t * (lineEnd - lineStart);
    return QLineF(point, closestPoint).length();
}

//Поиск ребра в точке
int GraphView::findEdgeAt(QPointF position)
{
    if (!graph) return -1;

    const auto& edges = graph->getEdges();
    double minDistance = minDistanceToFind;
    int closestEdgeId = -1;
    for(const auto& [id, edge]:edges)
    {
        QLineF line = edge->getLine();
        double distance = distanceToLine(position, line.p1(), line.p2());
        if(minDistance > distance)
        {
            closestEdgeId = id;
            minDistance = distance;
        }
    }
    return closestEdgeId;
}

//Обработка ЛКМ
void GraphView::handleLeftClick(int clickedNodeId, int clickedEdgeId, QPointF scenePos)
{
    if (clickedNodeId == -1) // Выбрали не узел
    {
        if(clickedEdgeId != -1)//Выбрали ребро
        {
            deselectNode();
            selectedEdgeId = clickedEdgeId;
            emit edgeSelected(clickedEdgeId);
        }
        else if (selectedNodeId != -1 or selectedEdgeId != -1) // не узел и не ребро, но что-то было выделено уже
            deselectAll();
        else  //клик в пустоту, когда ничего не выбрано
            emit clicked(scenePos);
    }
    else //выбран узел
    {
        deselectEdge();
        selectedNodeId = clickedNodeId;
        draggedNodeId = clickedNodeId;  // начинаем перетаскивание
        emit nodeSelected(clickedNodeId);
    }
}

//Обработка ПКМ
void GraphView::handleRightClick(int clickedNodeId)
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

//Обработка нажатия мышью
void GraphView::mousePressEvent(QMouseEvent* event)
{
    QPointF scenePos = mapToScene(event->pos());
    int clickedNodeId = findNodeAt(scenePos);
    int clickedEdgeId = findEdgeAt(scenePos);

    if (event->button() == Qt::LeftButton)
    {
        handleLeftClick(clickedNodeId, clickedEdgeId, scenePos);
    }
    else if (event->button() == Qt::RightButton)
    {
        if(clickedNodeId == -1)
            return;
        handleRightClick(clickedNodeId);
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
        node->setPosition(scenePos);
        drawGraph();
    }
}

//Отпустили кнопку мыши
void GraphView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && draggedNodeId != -1)
    {
        draggedNodeId = -1;
        emit moved();
    }
}
