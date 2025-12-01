#include "mainwindow.h"
#include <QToolBar>
#include <QLabel>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    controller = new GraphController(this);
    GraphView* view = controller->getView();

    tipsLabel = new TipsLabel(view);
    sidebarPanel = new SidebarPanel(view);
    setupUI();
    setupTipsConnections();
    onDeselected();
}

MainWindow::~MainWindow()
{
}

//Переключение скрытия подсказок
void MainWindow::toggleTipsVisible()
{
    if(isTipsVisible)
    {
        tipsLabel->hide();
        sidebarPanel->hide();
        isTipsVisible = false;
    }
    else
    {
        tipsLabel->show();
        sidebarPanel->show();
        isTipsVisible = true;
    }
}

//Обработка события изменения размера окна приложения
void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    if (tipsLabel)
        tipsLabel->updatePosition();
    if(sidebarPanel)
        sidebarPanel->updatePosition();

}

//Обработка события закрытия приложения
void MainWindow::closeEvent(QCloseEvent* event)
{
    bool shouldClose = controller->handleCloseEvent();
    if (shouldClose)
        event->accept();
    else
        event->ignore();
}

//Подготовка основного окна
void MainWindow::setupUI()
{
    GraphView* view = controller->getView();
    setCentralWidget(view);
    setWindowTitle("Редактор графов заводов и поставок");
    resize(800, 600);

    QToolBar* toolbar = addToolBar("Tools");

    QAction* tipsAction = toolbar->addAction("Подсказки");
    connect(tipsAction, &QAction::triggered, this, &MainWindow::toggleTipsVisible);

    QAction* infoAction = toolbar->addAction("Редактировать");
    connect(infoAction, &QAction::triggered, controller, &GraphController::showInfoForm);

    QAction* undoAction = toolbar->addAction("Назад");
    connect(undoAction, &QAction::triggered, controller, &GraphController::undo);

    QAction* redoAction = toolbar->addAction("Вперед");
    connect(redoAction, &QAction::triggered, controller, &GraphController::redo);

    QAction* saveAction = toolbar->addAction("Сохранить");
    connect(saveAction, &QAction::triggered, controller, &GraphController::saveGraph);

    QAction* loadAction = toolbar->addAction("Загрузить");
    connect(loadAction, &QAction::triggered, controller, &GraphController::loadGraph);

    QAction* removeNodeAction = toolbar->addAction("Удалить завод");
    connect(removeNodeAction, &QAction::triggered, controller, &GraphController::removeSelectedNode);

    QAction* removeEdgeAction = toolbar->addAction("Удалить поставку");
    connect(removeEdgeAction, &QAction::triggered, controller, &GraphController::removeSelectedEdge);

    QAction* clearAction = toolbar->addAction("Очистить");
    connect(clearAction, &QAction::triggered, controller, &GraphController::clearGraph);
}

//Подключение сигналов от GraphView к MainWindow
void MainWindow::setupTipsConnections()
{
    GraphView* view = controller->getView();
    connect(view, &GraphView::nodeSelected, this, &MainWindow::onNodeSelected);
    connect(view, &GraphView::edgeSelected, this, &MainWindow::onEdgeSelected);
    connect(view, &GraphView::deselected, this, &MainWindow::onDeselected);
}

//Обработка выбора узла
void MainWindow::onNodeSelected(int nodeId)
{
    Node* node = controller->getGraph()->getNode(nodeId);
    if (!node) return;
    tipsLabel->showNodeSelected();
    sidebarPanel->showNodeSelected(node);
}

//Обработка выбора ребра
void MainWindow::onEdgeSelected(int edgeId)
{
    Edge* edge = controller->getGraph()->getEdge(edgeId);
    if (!edge) return;

    tipsLabel->showEdgeSelected();
    sidebarPanel->showEdgeSelected(edge);
}

//Обработка снятия выбора
void MainWindow::onDeselected()
{
    tipsLabel->showNothingSelected();
    sidebarPanel->showNothingSelected();
}

