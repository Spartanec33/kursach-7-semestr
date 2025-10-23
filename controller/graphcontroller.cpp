#include "graphcontroller.h"

#include <QFileDialog>

GraphController::GraphController(Graph* model, GraphView* v, QObject* parent)
    : QObject(parent), graph(model), view(v)
{
    graphChanged();
    view->setGraph(graph);
    connect(view, &GraphView::clicked, this, &GraphController::addNodeAt);
    connect(view, &GraphView::edgeCreated, this, &GraphController::addEdge);
    connect(view, &GraphView::moved, this, &GraphController::graphChanged);
}

//Добавить узел в точку
void GraphController::addNodeAt(const QPointF& position)
{
    NodeData data;
    graph->addNode(data, position);
    graphChanged();
}

//Добавить ребро между узлами
void GraphController::addEdge(int fromId, int toId)
{
    EdgeData data;
    data.info = QString("Edge %1->%2").arg(fromId).arg(toId);
    graph->addEdge(data, fromId, toId);
    graphChanged();
}

// Удалить узел по его id
void GraphController::removeNode(int id)
{
    if (graph->removeNode(id))
        graphChanged();
}

// Удалить ребро по его id
void GraphController::removeEdge(int id)
{
    if (graph->removeEdge(id))
        graphChanged();
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
    graphChanged();
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
        graphChanged();
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
        graphChanged();
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
    else// Ничего не выделено
    {
        QMessageBox::information(nullptr, "Info",
            "Please select a node or edge first!\n\n"
            "• Click on node to select it\n"
            "• Click on edge to select it");
    }
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
            QDataStream output(&file);
            output << *graph;
            file.close();
        }
    }
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
            delete graph;
            graph = new Graph();
            view->deselectAll();

            QDataStream input(&file);
            input >> *graph;
            view->setGraph(graph);
            currentIndex = -1;
            graphChanged();
            file.close();
        }
    }
}

//Обработать изменение графа
void GraphController::graphChanged()
{
    if (currentIndex < (int)graphHistory.size() - 1)
        graphHistory.erase(graphHistory.begin() + currentIndex + 1, graphHistory.end());

    // Добавляем новое состояние
    graphHistory.push_back(*graph);
    currentIndex = graphHistory.size() - 1;

    // Обновляем представление
    view->drawGraph();
}

//Шаг в прошлое в истории графов
void GraphController::undo()
{
    view->deselectAll();
    if(currentIndex > 0)
    {
        *graph = graphHistory[--currentIndex];
        view->drawGraph();
    }
}

//Шаг в будущее в истории графов
void GraphController::redo()
{
    view->deselectAll();
    if(currentIndex < graphHistory.size()-1)
    {
        *graph = graphHistory[++currentIndex];
        view->drawGraph();
    }
}








