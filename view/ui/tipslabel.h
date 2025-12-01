#ifndef TIPSLABEL_H
#define TIPSLABEL_H

#include <QLabel>
#include <QWidget>

class TipsLabel : public QLabel
{
    Q_OBJECT
public:
    explicit TipsLabel(QWidget* parent = nullptr);

    void showNodeSelected();    //Установить подсказку при выделенном узле
    void showEdgeSelected();    //Установить подсказку при выделенном ребре
    void showNothingSelected(); //Установить подсказку для состояния, когда ничего не выделено

    void updatePosition();      //Сдвинуть в правый верхний угол

private:
    void setupStyle();           //Установка стиля виджета
    void applyTip(QString tips); //Установка подсказки
};

#endif // TIPSLABEL_H
