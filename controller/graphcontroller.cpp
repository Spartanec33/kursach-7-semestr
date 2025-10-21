#include "graphcontroller.h"

#include <QFileDialog>

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
    graph->addNode(data, position);
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
    view->deselectAll();
}

// Удалить выбранное ребро
void GraphController::removeSelectedEdge()
{
    removeEdge(view->getSelectedEdge());
    view->deselectAll();
}

//Очистить граф
void GraphController::clearGraph()
{
    graph->clear();
    view->drawGraph();
}

// Обработка информационного окошка узла
void GraphController::processNodeForm(int selectedNodeId)
{
    Node* node = graph->getNode(selectedNodeId);
    if (!node) return;

    NodeForm form(nullptr);
    form.setName(node->getData().name);

    if (form.exec() == QDialog::Accepted)
    {
        NodeData data = node->getData();
        data.name = form.getName();
        node->setData(data);
    }
}

// Обработка информационного окошка ребра
void GraphController::processEdgeForm(int selectedEdgeId)
{
    Edge* edge = graph->getEdge(selectedEdgeId);
    if (!edge) return;

    EdgeForm form(nullptr);
    form.setInfo(edge->getData().info);

    if (form.exec() == QDialog::Accepted)
    {
        EdgeData data = edge->getData();
        data.info = form.getInfo();
        edge->setData(data);
    }
}

//Показать окно информации
void GraphController::showInfoForm()
{
    // Проверяем что выделено
    int selectedNodeId = view->getSelectedNode();
    int selectedEdgeId = view->getSelectedEdge();

    if (selectedNodeId != -1) //Выделен узел
    {
        processNodeForm(selectedNodeId);
    }
    else if (selectedEdgeId != -1) //Выделено ребро
    {
        processEdgeForm(selectedEdgeId);
    }
    else // Ничего не выделено
    {
        QMessageBox::information(nullptr, "Info",
            "Please select a node or edge first!\n\n"
            "• Click on node to select it\n"
            "• Click on edge to select it");
    }
    view->drawGraph();
}

// Сереализация графа
void GraphController::serializeGraph(QFile& file)
{
    QDataStream output(&file);

    const auto& nodes = graph->getNodes();
    int nodesCount = nodes.size();
    output << nodesCount;
    for (const auto& [id, node] : nodes)
        output << *node;

    const auto& edges = graph->getEdges();
    int edgesCount = edges.size();
    output << edgesCount;
    for (const auto& [id, edge] : edges)
        output << *edge;
}

//Сохранить граф
void GraphController::saveGraph()
{
    QFileDialog dialog(nullptr);
    dialog.setWindowTitle("Save Graph");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter("Binary Files (*.bin)");
    if (dialog.exec() == QDialog::Accepted)
    {
        QString fileName = dialog.selectedFiles().first();
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly))
        {
            serializeGraph(file);
            file.close();
        }
    }
}

// Десереализация графа
void GraphController::deserializeGraph(QFile& file)
{
    delete graph;
    graph = new Graph();
    view->deselectAll();

    QDataStream input(&file);

    //Узлы
    int nodeCount;
    Node node;
    input >> nodeCount;
    for(int i = 0; i < nodeCount; i++)
    {
        input >> node;
        graph->addNode(node);
    }

    //Ребра
    int edgeCount;
    Edge edge;
    input >> edgeCount;
    for(int i = 0; i < edgeCount; i++)
    {
        input >> edge;
        graph->addEdge(edge);
    }

    view->setGraph(graph);
}

//Загрузить граф
void GraphController::loadGraph()
{
    QFileDialog dialog(nullptr);
    dialog.setWindowTitle("Load Graph");
    dialog.setNameFilter("Binary Files (*.bin)");
    if (dialog.exec() == QDialog::Accepted)
    {
        QString fileName = dialog.selectedFiles().first();
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly))
        {
            deserializeGraph(file);
            file.close();
        }
    }
}


