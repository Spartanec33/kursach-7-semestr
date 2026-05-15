#include "tipslabel.h"
#include <QResizeEvent>

TipsLabel::TipsLabel(QWidget* parent)
    : QLabel(parent)
{
    setupStyle();
    showNothingSelected();
}

//Установка стиля виджета
void TipsLabel::setupStyle()
{
    setStyleSheet(
        "TipsLabel {"
        "   background-color: rgba(0, 0, 0, 150);"
        "   color: white;"
        "   padding: 10px;"
        "   border-radius: 5px;"
        "   font-size: 12px;"
        "   line-height: 130%;"
        "}"
    );

    setAlignment(Qt::AlignTop | Qt::AlignLeft);
}

//Установка подсказки
void TipsLabel::applyTip(QString tips)
{
    setText(tips);
    adjustSize();
    updatePosition();
}

//Установить подсказку при выделенном узле
void TipsLabel::showNodeSelected()
{
    QString tips =
        " <b>Выбран узел</b><br><br>"
        "• Нажмите на кнопку редактирования - для редактирования<br>"
        "• Зажать ЛКМ на текущем узле + двигать - перемещение узла<br>"
        "• ЛКМ на другом узле - выбрать его<br>"
        "• ПКМ на другом узле - создать связь между ними<br>"
        "• ЛКМ на связи - выбрать её<br>"
        "• ЛКМ в пустое место - снять выделение";

    applyTip(tips);
}

//Установить подсказку при выделенном ребре
void TipsLabel::showEdgeSelected()
{
    QString tips =
        " <b>Выбрана связь</b><br><br>"
        "• Нажмите на кнопку редактирования - для редактирования<br>"
        "• ЛКМ на другой связи - выбрать её<br>"
        "• ЛКМ/ПКМ на узле - выбрать его<br>"
        "• ЛКМ в пустое место - снять выделение";

    applyTip(tips);
}

//Установить подсказку для состояния, когда ничего не выделено
void TipsLabel::showNothingSelected()
{
    QString tips =
        "🎯 <b>Ничего не выбрано</b><br><br>"
        "• ЛКМ в пустое место - создать узел<br>"
        "• ЛКМ по связи - выбрать ее<br>"
        "• ЛКМ/ПКМ по узлу - выбрать его";

    applyTip(tips);
}

//Сдвинуть в правый верхний угол
void TipsLabel::updatePosition()
{
    if (parentWidget())
        move(parentWidget()->width() - width() - 10, 10);
}
