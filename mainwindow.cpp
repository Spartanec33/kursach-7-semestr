#include "mainwindow.h"
#include <QToolBar>
#include <QAction>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    graph = new Graph();
    view = new GraphView(this);
    controller = new GraphController(graph, view, this);

    view->setGraph(graph);
    setupUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    bool shouldClose = controller->handleCloseEvent();
    if (shouldClose)
        event->accept();
    else
        event->ignore();
}

void MainWindow::setupUI()
{
    setCentralWidget(view);
    setWindowTitle("Редактор графов заводов и поставок");
    resize(800, 600);

    QToolBar* toolbar = addToolBar("Tools");

    QAction* removeNodeAction = toolbar->addAction("Удалить завод");
    connect(removeNodeAction, &QAction::triggered, controller, &GraphController::removeSelectedNode);

    QAction* removeEdgeAction = toolbar->addAction("Удалить поставку");
    connect(removeEdgeAction, &QAction::triggered, controller, &GraphController::removeSelectedEdge);

    QAction* infoAction = toolbar->addAction("Информация");
    connect(infoAction, &QAction::triggered, controller, &GraphController::showInfoForm);

    QAction* undoAction = toolbar->addAction("Назад");
    connect(undoAction, &QAction::triggered, controller, &GraphController::undo);

    QAction* redoAction = toolbar->addAction("Вперед");
    connect(redoAction, &QAction::triggered, controller, &GraphController::redo);

    QAction* saveAction = toolbar->addAction("Сохранить");
    connect(saveAction, &QAction::triggered, controller, &GraphController::saveGraph);

    QAction* loadAction = toolbar->addAction("Загрузить");
    connect(loadAction, &QAction::triggered, controller, &GraphController::loadGraph);

    QAction* clearAction = toolbar->addAction("Очистить");
    connect(clearAction, &QAction::triggered, controller, &GraphController::clearGraph);
}
