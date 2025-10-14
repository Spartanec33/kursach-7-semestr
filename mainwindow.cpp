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

void MainWindow::setupUI()
{
    setCentralWidget(view);
    setWindowTitle("Graph Editor");
    resize(800, 600);

    QToolBar* toolbar = addToolBar("Tools");

    QAction* removeNodeAction = toolbar->addAction("Remove Node");
    connect(removeNodeAction, &QAction::triggered, controller, &GraphController::removeSelectedNode);

    QAction* removeEdgeAction = toolbar->addAction("Remove Edge");
    connect(removeEdgeAction, &QAction::triggered, controller, &GraphController::removeSelectedEdge);
}
