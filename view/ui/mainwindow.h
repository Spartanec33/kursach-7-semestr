#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include "graphview.h"
#include "graphcontroller.h"
#include "tipslabel.h"
#include "sidebarpanel.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNodeSelected(int nodeId);    //Обработка выбора узла
    void onEdgeSelected(int edgeId);    //Обработка выбора ребра
    void onDeselected();                //Обработка снятия выбора

private:
    void setupUI();               //Подготовка основного окна
    void setupTipsConnections();  //Подключение сигналов от GraphView к MainWindow
    void toggleTipsVisible();     //Переключение скрытия подсказок
    void closeEvent(QCloseEvent* event) override;   //Обработка события изменения размера окна приложения
    void resizeEvent(QResizeEvent* event) override; //Обработка события закрытия приложения

    GraphController* controller;
    TipsLabel* tipsLabel;
    SidebarPanel* sidebarPanel;
    bool isTipsVisible = true;
};

#endif // MAINWINDOW_H
