#ifndef SIDEBARPANEL_H
#define SIDEBARPANEL_H

#include <QWidget>
#include "node.h"
#include "edge.h"

class SidebarPanel : public QWidget
{
    Q_OBJECT
public:
    explicit SidebarPanel(QWidget* parent = nullptr);

    void showNodeSelected(Node* node); //Показать виджет с информацией о узле
    void showEdgeSelected(Edge* edge, Node* source); //Показать виджет с информацией о ребре
    void showNothingSelected();        //Показать виджет при состоянии, когда ничего не выделено
    void updatePosition();  //Сдвинуть в левый верхний угол
    void clear();           //Убрать виджет с панели

private:
    void setupStyle(); //Установка стиля
    QWidget* currentWidget = nullptr; //Установленный виджет
};

#endif // SIDEBARPANEL_H
