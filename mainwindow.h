#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include "graphview.h"
#include "graphcontroller.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void closeEvent(QCloseEvent* event) override;

    Graph* graph;
    GraphView* view;
    GraphController* controller;
};

#endif // MAINWINDOW_H
