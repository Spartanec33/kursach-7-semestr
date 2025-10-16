#ifndef STYLESHEET_H
#define STYLESHEET_H
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>

//красота в узлах
const int nodeSize = 80; //размер узлаv
const QColor nodeTextColor = Qt::red; //цвет текста в узле
const QColor nodeColor = Qt::blue; //цвет узла
const QColor selectedNodeColor = Qt::yellow; //цвет выделенного узла
const int borderFat = 2; //ширина границ узла
const QColor borderColor = Qt::black; //цвет границ узла

//красота в ребрах
const int edgeFat = 5; //ширина ребра
const QColor edgeColor = Qt::black; //цвет ребра
const double edgeOffset = 20; //смещение ребер
const int arrowSize = 30;  //размер стрелки
const QColor arrowColor = Qt::red; //цвет стрелки
const QColor selectedArrowColor = Qt::green; //цвет ребра

#endif // STYLESHEET_H
