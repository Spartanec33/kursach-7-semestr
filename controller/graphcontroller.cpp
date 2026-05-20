#include "graphcontroller.h"
#include <QFileDialog>
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>

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
    data.weight = 0;
    if(!graph->addEdge(data, fromId, toId))
    {
        QMessageBox::warning(nullptr, "Ошибка", "Невозможно создать связь");
        return;
    }
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
            "Сначала выделите узел");
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
            "Сначала выделите ребро");
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

void GraphController::processNodeForm(int selectedNodeId)
{
    Node* node = graph->getNode(selectedNodeId);
    if (!node) return;

    NodeForm* form = new NodeForm();
    form->setName(node->getData().name);

    QScopedPointer<QDialog> dialog(createDialog("Сведения о узле", form));

    while (dialog->exec() == QDialog::Accepted)
    {
        QString newName = form->getName();

        if (graph->isNameExists(newName, node->getId()))
        {
            QMessageBox::warning(nullptr, "Ошибка", "Узел с таким названием уже существует!");
            continue;
        }

        NodeData data = node->getData();
        data.name = newName;
        node->setData(data);
        graphChanged();
        break;
    }
}

// Обработка информационного окошка ребра
void GraphController::processEdgeForm(int selectedEdgeId)
{
    Edge* edge = graph->getEdge(selectedEdgeId);
    if (!edge) return;

    Node* sourceNode = graph->getNode(edge->getSourceId());
    if (!sourceNode) return;

    EdgeForm* form = new EdgeForm();
    form->setWeight(edge->getData().weight);

    QScopedPointer<QDialog> dialog(createDialog("Сведения о ребре", form));
    if (dialog->exec() == QDialog::Accepted)
    {
        EdgeData data = edge->getData();
        data.weight = form->getWeight();
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
    bool isHighlighted = view->getIsHighlighted();
    if(isHighlighted) //Выделен путь
    {
        QMessageBox::information(nullptr, "Предупреждение",
            "Выделен путь\n\n"
            "• Нажмите в любое место поля, чтобы снять выделение");
    }
    else if (selectedNodeId != -1) //Выделен узел
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
            "Сначала выделите узел или ребро\n\n"
            "• Нажмите на узел, чтобы выделить узел\n"
            "• Нажмите на ребро, чтобы выделить ребро");
    }
}

//Показать диалог для решения задачи ДП
void GraphController::showSolveDialog()
{
    if(graph->hasAnyCycle())
    {
        QMessageBox::warning(nullptr, "Ошибка", "Граф содержит циклы, решение методом ДП невозможно");
        return;
    }
    if(!graph->getNodes().size())
    {
        QMessageBox::warning(nullptr, "Ошибка", "Граф пуст");
        return;
    }
    QDialog dialog;
    dialog.setWindowTitle("Поиск кратчайшего пути");

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    // Создаем списки выбора (ComboBox)
    QComboBox* startCombo = new QComboBox();
    QComboBox* endCombo = new QComboBox();

    // Заполняем их именами всех узлов
    for (auto const& [id, node] : graph->getNodes())
    {
        QString name = node->getData().name;
        startCombo->addItem(name, id);
        endCombo->addItem(name, id);
    }

    layout->addWidget(new QLabel("Выберите начальный узел:"));
    layout->addWidget(startCombo);
    layout->addWidget(new QLabel("Выберите конечный узел:"));
    layout->addWidget(endCombo);

    QPushButton* solveBtn = new QPushButton("Рассчитать");
    layout->addWidget(solveBtn);

    connect(solveBtn, &QPushButton::clicked, [&]()
    {
        int idFrom = startCombo->currentData().toInt();
        int idTo = endCombo->currentData().toInt();
        dialog.accept();
        solvePathWithExport(idFrom, idTo);
    });

    dialog.exec();
}

void GraphController::solvePathWithExport(int startId, int endId)
{
    int resDist = 0;
    QString resPath = "";
    auto [path, logText] = graph->findShortestPathWithLog(startId, endId, resDist, resPath);

    if (path.empty())
    {
        QMessageBox::warning(nullptr, "Результат", "Путь не найден. Лог не будет сохранен.");
        return;
    }
    else
    {
        QString resultMessage = QString("Итоговый путь: %1\nОбщая стоимость: %2")
                                            .arg(resPath)
                                            .arg(resDist);

        QMessageBox::information(nullptr, "Путь найден", resultMessage);
    }

    // Предлагаем сохранить лог в файл
    QMessageBox askBox(QMessageBox::Question, "Сохранение отчета",
                           "Желаете сохранить подробный протокол решения в текстовый файл?",
                           QMessageBox::Yes | QMessageBox::No);

    askBox.setButtonText(QMessageBox::Yes, "Да");
    askBox.setButtonText(QMessageBox::No, "Нет");
    if (askBox.exec() == QMessageBox::Yes)
    {
        QString fileName = QFileDialog::getSaveFileName(nullptr,
            "Сохранить отчет о решении", "", "Text Files (*.txt)");

        if (!fileName.isEmpty())
        {
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);
                out << logText;
                file.close();
                QMessageBox::information(nullptr, "Успех", "Отчет успешно сохранен в файл.");
            }
        }
    }
    view->highlightPath(path);
}



//Сохранить граф
void GraphController::saveGraph()
{
    QFileDialog dialog(nullptr);
    dialog.setWindowTitle("Save Graph");
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter("Text Files (*.txt)");
    if (dialog.exec() == QDialog::Accepted)
    {
        QString fileName = dialog.selectedFiles().first();
        QFile file(fileName);
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream output(&file);
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
    dialog.setNameFilter("Text Files (*.txt);;All Files (*)");

    if (dialog.exec() == QDialog::Accepted)
    {
        QString fileName = dialog.selectedFiles().first();
        QFile file(fileName);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream input(&file);
            Graph* newGraph = new Graph();
            input >> *newGraph;

            // Проверка на адекватность данных
            if (input.status() != QTextStream::Ok)
            {
                QMessageBox::critical(nullptr, "Ошибка загрузки",
                    "Файл содержит некорректные данные!\n\n"
                    "Возможные причины:\n"
                    "- Узлы имеют одинаковые имена\n"
                    "- Обнаружены связи в обе стороны\n"
                    "- Присутствуют отрицательные веса\n"
                    "- Нарушена структура текстового файла");

                delete newGraph;
                file.close();
                return;
            }

            // Если данные валидны — обновляем рабочее состояние
            view->setGraph(nullptr);
            view->deselectAll();

            if (graph)
                delete graph;

            graph = newGraph;
            view->setGraph(graph); // Подключаем новый валидный граф

            // Сброс истории правок и уведомление системы об изменениях
            graphHistory.clear();
            currentIndex = -1;
            graphChanged();

            file.close();
        }
        else
        {
            QMessageBox::warning(nullptr, "Ошибка", "не удалось открыть файл для чтения.");
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






