#ifndef STYLESHEET_H
#define STYLESHEET_H
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>

//красота в узлах
const int nodeSize = 80;                     //размер узла
const QColor nodeTextColor = Qt::black;      //цвет текста в узле
const int textSize = 10;                     //размер шрифта
const QColor nodeColor = Qt::gray   ;        //цвет узла
const QColor selectedNodeColor = Qt::yellow; //цвет выделенного узла
const int borderFat = 2;                     //ширина границ узла
const QColor borderColor = Qt::black;        //цвет границ узла

//красота в ребрах
const int edgeFat = 5;                       //ширина ребра
const QColor edgeColor = Qt::black;          //цвет ребра
const QColor selectedEdgeColor = Qt::red;    //цвет выделенного ребра
const double edgeOffset = 20;                //смещение ребер
const int arrowSize = 30;                    //размер стрелки
const QColor arrowColor = Qt::red;           //цвет стрелки
const QColor selectedArrowColor = Qt::green; //цвет выделенной стрелки
const double minDistanceToFind = 15;         //Минимальное расстояние для поиска ребра при клике

#endif // STYLESHEET_H
