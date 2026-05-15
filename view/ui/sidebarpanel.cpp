#include "sidebarpanel.h"
#include "nodeform.h"
#include "edgeform.h"
#include <QVBoxLayout>
#include <QLabel>

SidebarPanel::SidebarPanel(QWidget* parent) : QWidget(parent), currentWidget(nullptr)
{
    setupStyle();
    showNothingSelected();
}

//Установка стиля
void SidebarPanel::setupStyle()
{
    setStyleSheet(
        "SidebarPanel {"
        "   background-color: rgb(245, 245, 245, 230);"
        "   border-left: 2px solid rgba(200, 200, 200, 150);"
        "}"
    );
    setFixedWidth(250);
    setFixedHeight(100);
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    QLabel* header = new QLabel("📋 Информация", this);
    header->setAlignment(Qt::AlignCenter);
    header->setFixedHeight(40); // Фиксированная высота
    header->setStyleSheet(
        "QLabel {"
        "   background-color: rgb(224, 224, 224);" // Серый без прозрачности
        "   color: black;" // Черный текст для серого фона
        "   padding: 12px;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "   border-bottom: 2px solid rgba(180, 180, 180, 150);"
        "   border-radius: 0;" // Убрали скругление
        "}"
    );
    layout->addWidget(header);
}

//Показать виджет с информацией о узле
void SidebarPanel::showNodeSelected(Node* node)
{
    clear();

    NodeForm* form = new NodeForm(this);
    form->setName(node->getData().name);
    form->setReadOnly(true);

    currentWidget = form;
    layout()->addWidget(form);
}

//Показать виджет с информацией о ребре
void SidebarPanel::showEdgeSelected(Edge* edge, Node* source)
{
    clear();

    EdgeForm* form = new EdgeForm(this);
    form->setWeight(edge->getData().weight);
    form->setReadOnly(true);

    currentWidget = form;
    layout()->addWidget(form);
}

//Показать виджет при состоянии, когда ничего не выделено
void SidebarPanel::showNothingSelected()
{
    clear();

    QLabel* emptyLabel = new QLabel("Выберите элемент", this);
    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setWordWrap(true);
    emptyLabel->setStyleSheet(
        "QLabel {"
        "   color: #ccc;"  // Светлее для темного фона
        "   padding: 40px 20px;"
        "   font-style: italic;"
        "   font-size: 13px;"
        "}"
    );

    currentWidget = emptyLabel;
    layout()->addWidget(emptyLabel);
}

//Сдвинуть в левый верхний угол
void SidebarPanel::updatePosition()
{
    if (parentWidget())
        move(10, 10);
}

//Убрать виджет с панели
void SidebarPanel::clear()
{
    if (currentWidget)
    {
        layout()->removeWidget(currentWidget);
        delete currentWidget;
        currentWidget = nullptr;
    }
}
