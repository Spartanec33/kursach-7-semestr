#include "graphview.h"
#include <QMouseEvent>
#include "config.h"
//#include "styleSheet.h"

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
    line.translate(perpendicular * edgeOffset());

    return line;
}

void GraphView::drawArrow(QLineF line, bool isSelected, double weight)
{
    // Вычисляем угол линии
    double angle = atan2(line.dy(), -line.dx());

    // Точка для основания стрелки (посередине линии)
    QPointF arrowBase = line.pointAt(0.5);

    // Создаем точки для стрелки
    QPointF arrowP1 = arrowBase + QPointF(
                sin(angle + M_PI / 3) * arrowSize(),
                cos(angle + M_PI / 3) * arrowSize()
                );

    QPointF arrowP2 = arrowBase + QPointF(
               sin(angle + M_PI - M_PI / 3) * arrowSize(),
                cos(angle + M_PI - M_PI / 3) * arrowSize()
                );

    // Создаем полигон для стрелки
    QPolygonF arrowHead;
    arrowHead << arrowBase << arrowP1 << arrowP2;

    QBrush brush = (isSelected) ? QBrush(selectedArrowColor()) : QBrush(arrowColor());

    // Добавляем стрелку на сцену
    scene->addPolygon(arrowHead, QPen(edgeColor(), edgeWidth()), brush);

    // Отрисовка веса ребра
    // Создаем текстовый элемент
    auto* text = scene->addText(QString::number(weight));
    text->setDefaultTextColor(isSelected ? selectedEdgeColor() : edgeColor());
    text->setFont(QFont("Arial", textSize() - 2)); // Сделаем шрифт чуть меньше узлов

    // Вычисляем перпендикуляр к линии, чтобы отодвинуть текст вбок
    QPointF direction = line.p2() - line.p1();
    QPointF perpendicular(-direction.y(), direction.x());

    double length = sqrt(perpendicular.x() * perpendicular.x() + perpendicular.y() * perpendicular.y());
    if (length > 0)
        perpendicular /= length; // Нормализуем

    // Сдвигаем текст на 15 пикселей вбок от центра стрелки
    QPointF textCenter = arrowBase + perpendicular * 15;

    // Центрируем bounding box текста относительно вычисленной точки
    QRectF textRect = text->boundingRect();
    text->setPos(textCenter.x() - textRect.width() / 2,
                 textCenter.y() - textRect.height() / 2);
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

        QLineF line = applyEdgeOffset(sourceNode->getPosition(), targetNode->getPosition());
        edge->setLine(line);

        QColor color = (id == selectedEdgeId) ? selectedEdgeColor() : edgeColor();

        // Сохраняем указатель на созданную линию
        QGraphicsLineItem* lineItem = scene->addLine(line, QPen(color, edgeWidth()));

        // Привязываем ID ребра к этой линии
        // Используем ключ 0 для ID самого ребра.
        // Также запишем ID начального и конечного узлов в ключи 1 и 2
        lineItem->setData(0, id);
        lineItem->setData(1, edge->getSourceId());
        lineItem->setData(2, edge->getTargetId());

        // 3. Рисуем стрелку и вес
        drawArrow(line, id == selectedEdgeId, edge->getData().weight);
    }
}

// Отрисовать текст в узле
void GraphView::drawNodeText(QPointF center, QString name)
{
    auto* text = scene->addText(name);
    QRectF textRect = text->boundingRect();
    text->setPos(center.x() - textRect.width()/2,
                 center.y() - textRect.height()/2);
    text->setDefaultTextColor(nodeTextColor());
    text->setFont(QFont("Arial", textSize()));
}

// Отрисовать узлы
void GraphView::drawNodes()
{
    const auto& nodes = graph->getNodes();
    for (const auto& [id, node] : nodes)
    {
        QPointF center = node->getPosition();
        QBrush brush = (id == selectedNodeId) ? QBrush(selectedNodeColor()) : QBrush(nodeColor()); // Цвет в зависимости от выделенности

        //Сохраняем указатель на созданный эллипс
        QGraphicsEllipseItem* ellipse = scene->addEllipse(
            center.x() - nodeSize(),
            center.y() - nodeSize(),
            nodeSize() * 2,
            nodeSize() * 2,
            QPen(borderColor(), edgeWidth()),
            brush
        );

        //привязываем ID узла к этому графическому элементу
        ellipse->setData(0, id);

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

void GraphView::deselectAll()
{
    selectedNodeId = -1;
    selectedEdgeId = -1;
    emit deselected();
}

//Подсветить путь
void GraphView::highlightPath(const vector<int> &path)
{
    deselectAll();
    drawGraph();
    if (path.empty()) return;

    // Создаем множества для быстрого поиска ID, входящих в путь
    set<int> pathNodes(path.begin(), path.end());

    // Создаем пары (sourceId, targetId) для ребер, входящих в путь
    set<pair<int, int>> pathEdges;
    for (size_t i = 0; i < path.size() - 1; ++i)
    {
        pathEdges.insert({path[i], path[i+1]});
    }

    // Проходим по всем элементам на сцене
    for (QGraphicsItem* item : scene->items())
    {
        // 1. Если это узел
        if (auto* ellipse = dynamic_cast<QGraphicsEllipseItem*>(item))
        {
            QVariant nodeData = ellipse->data(0);
            if (nodeData.isValid())
            {
                int nodeId = nodeData.toInt();
                if (pathNodes.count(nodeId))
                {
                    ellipse->setPen(borderColor());
                    ellipse->setBrush(selectedNodeColor());
                }
            }
        }

        // 2. Если это ребро
        if (auto* lineItem = dynamic_cast<QGraphicsLineItem*>(item))
        {
            QVariant srcData = lineItem->data(1);
            QVariant dstData = lineItem->data(2);

            if (srcData.isValid() && dstData.isValid())
            {
                int srcId = srcData.toInt();
                int dstId = dstData.toInt();

                // Проверяем, входит ли эта направленная пара в путь
                if (pathEdges.count({srcId, dstId}))
                {
                    lineItem->setPen(QPen(selectedEdgeColor(), edgeWidth()));
                }
            }
        }
    }

    isHighlighted = true;
    emit pathHighlighted();
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
        if (QLineF(nodePos, position).length() < nodeSize())
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
    double minDistance = minDistanceToFind();
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
        deselectAll();
    }
}

//Обработка нажатия мышью
void GraphView::mousePressEvent(QMouseEvent* event)
{
    //Сброс подсветки если была
    if(isHighlighted)
    {
        isHighlighted = false;
        deselectAll();
        drawGraph();
        return;
    }

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
