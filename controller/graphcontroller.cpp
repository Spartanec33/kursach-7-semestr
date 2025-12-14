#include "graphcontroller.h"
#include <QFileDialog>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>

GraphController::GraphController(QObject* parent)
    : QObject(parent)
{
    graph = new Graph();
    view = new GraphView();

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
    else
    {
        QMessageBox::information(nullptr, "Предупреждение",
            "Сначала выделите завод");
    }
}

// Удалить ребро по его id
void GraphController::removeEdge(int id)
{
    if (graph->removeEdge(id))
        graphChanged();
    else
    {
        QMessageBox::information(nullptr, "Предупреждение",
            "Сначала выделите поставку");
    }
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
    view->deselectAll();
    graphChanged();
}

// Общая функция создания диалога с кнопками
QDialog* GraphController::createDialog(const QString& title, QWidget* content)
{
    QDialog* dialog = new QDialog(nullptr);
    dialog->setWindowTitle(title);
    dialog->resize(400, 240);

    QVBoxLayout* layout = new QVBoxLayout(dialog);
    layout->addWidget(content);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("ОК", dialog);
    QPushButton* cancelButton = new QPushButton("Отмена", dialog);

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, dialog, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, dialog, &QDialog::reject);

    return dialog;
}

// Обработка информационного окошка узла
void GraphController::processNodeForm(int selectedNodeId)
{
    Node* node = graph->getNode(selectedNodeId);
    if (!node) return;

    NodeForm* form = new NodeForm();
    form->setName(node->getData().name);
    form->setInfo(node->getData().info);
    form->setProducts(node->getData().products);

    QScopedPointer<QDialog> dialog(createDialog("Сведения о заводе", form));
    if (dialog->exec() == QDialog::Accepted)
    {
        NodeData data = node->getData();
        data.name = form->getName();
        data.info = form->getInfo();
        data.products = form->getProducts();

        node->setData(data);
        graphChanged();
    }
}

// Обработка информационного окошка поставки
void GraphController::processEdgeForm(int selectedEdgeId)
{
    Edge* edge = graph->getEdge(selectedEdgeId);
    if (!edge) return;

    Node* sourceNode = graph->getNode(edge->getSourceId());
    if (!sourceNode) return;

    EdgeForm* form = new EdgeForm();
    form->setInfo(edge->getData().info);
    form->setProducts(edge->getData().products);
    form->setAvailableProducts(sourceNode->getData().products);

    QScopedPointer<QDialog> dialog(createDialog("Сведения о поставке", form));
    if (dialog->exec() == QDialog::Accepted)
    {
        EdgeData data = edge->getData();
        data.info = form->getInfo();
        data.products = form->getProducts();

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
        view->nodeSelected(selectedNodeId);
    }
    else if (selectedEdgeId != -1) //Выделено ребро
    {
        processEdgeForm(selectedEdgeId);
        view->edgeSelected(selectedEdgeId);
    }
    else// Ничего не выделено
    {
        QMessageBox::information(nullptr, "Предупреждение",
            "Сначала выделите завод или поставку\n\n"
            "• Нажмите на узел, чтобы выделить завод\n"
            "• Нажмите на ребро, чтобы выделить поставку");
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
            view->setGraph(nullptr);
            view->deselectAll();

            Graph* newGraph = new Graph();

            QDataStream input(&file);
            input >> *newGraph;
            if (graph)
                delete graph;
            graph = newGraph;
            view->setGraph(graph);
            graphHistory.clear();
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
    else
    {
        QMessageBox::information(nullptr, "Предупреждение",
            "Больше идти назад некуда.");
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
    else
    {
        QMessageBox::information(nullptr, "Предупреждение",
            "Больше идти вперед некуда.");
    }
}

//Обработать закрытие программы
bool GraphController::handleCloseEvent()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Выход");
    msgBox.setText("Сохранить изменения перед выходом?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setButtonText(QMessageBox::Save, "Сохранить");
    msgBox.setButtonText(QMessageBox::Discard, "Не сохранять");
    msgBox.setButtonText(QMessageBox::Cancel, "Отмена");

    int result = msgBox.exec();

    switch (result)
    {
        case QMessageBox::Save:
            saveGraph();
            return true;
        case QMessageBox::Discard:
            return true;
        case QMessageBox::Cancel:
            return false;
    }
}








