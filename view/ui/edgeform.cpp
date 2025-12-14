#include "edgeform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

EdgeForm::EdgeForm(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Описание поставки
    mainLayout->addWidget(new QLabel("Описание поставки:"));
    infoEdit = new QPlainTextEdit;
    infoEdit->setMaximumHeight(60);
    mainLayout->addWidget(infoEdit);

    // Информационная строка
    infoLabel = new QLabel("Выберите изделия для поставки");
    infoLabel->setStyleSheet("padding: 5px; background: #f0f0f0; border-radius: 3px;");
    mainLayout->addWidget(infoLabel);

    // Два списка с кнопками между ними
    QHBoxLayout* listsLayout = new QHBoxLayout;

    // Левый список: доступные изделия
    QVBoxLayout* leftLayout = new QVBoxLayout;
    leftLayout->addWidget(new QLabel("Доступно:"));
    availableList = new QListWidget;
    leftLayout->addWidget(availableList);

    // Кнопки для левого списка
    QHBoxLayout* leftButtons = new QHBoxLayout;
    addAllButton = new QPushButton(">> Все");
    leftButtons->addWidget(addAllButton);
    leftButtons->addStretch();
    leftLayout->addLayout(leftButtons);

    listsLayout->addLayout(leftLayout);

    // Центральные кнопки
    QVBoxLayout* centerButtons = new QVBoxLayout;
    centerButtons->addStretch();
    addButton = new QPushButton(">");
    addButton->setFixedWidth(40);
    removeButton = new QPushButton("<");
    removeButton->setFixedWidth(40);
    centerButtons->addWidget(addButton);
    centerButtons->addWidget(removeButton);
    centerButtons->addStretch();
    listsLayout->addLayout(centerButtons);

    // Правый список: выбранные изделия
    QVBoxLayout* rightLayout = new QVBoxLayout;
    rightLayout->addWidget(new QLabel("Поставляется:"));
    selectedList = new QListWidget;
    rightLayout->addWidget(selectedList);

    // Кнопки для правого списка
    QHBoxLayout* rightButtons = new QHBoxLayout;
    removeAllButton = new QPushButton("Все <<");
    rightButtons->addStretch();
    rightButtons->addWidget(removeAllButton);
    rightLayout->addLayout(rightButtons);

    listsLayout->addLayout(rightLayout);
    mainLayout->addLayout(listsLayout);

    connect(addButton, &QPushButton::clicked, this, &EdgeForm::onAddProduct);
    connect(removeButton, &QPushButton::clicked, this, &EdgeForm::onRemoveProduct);
    connect(addAllButton, &QPushButton::clicked, this, &EdgeForm::onAddAll);
    connect(removeAllButton, &QPushButton::clicked, this, &EdgeForm::onRemoveAll);
}

void EdgeForm::onAddProduct()
{
    QListWidgetItem* currentItem = availableList->currentItem();
    if (currentItem)
    {
        // Переносим из доступных в выбранные
        QString product = currentItem->text();
        delete currentItem;
        selectedList->addItem(product);

        // Обновляем информацию
        infoLabel->setText(QString("Выбрано %1 изделий").arg(selectedList->count()));
    }
}

void EdgeForm::onRemoveProduct()
{
    QListWidgetItem* currentItem = selectedList->currentItem();
    if (currentItem)
    {
        // Возвращаем из выбранных в доступные
        QString product = currentItem->text();
        delete currentItem;
        availableList->addItem(product);

        // Обновляем информацию
        infoLabel->setText(QString("Выбрано %1 изделий").arg(selectedList->count()));
    }
}

void EdgeForm::onAddAll()
{
    // Переносим все из доступных в выбранные
    while (availableList->count() > 0)
    {
        QListWidgetItem* item = availableList->takeItem(0);
        selectedList->addItem(item->text());
        delete item;
    }
    infoLabel->setText(QString("Выбрано %1 изделий").arg(selectedList->count()));
}

void EdgeForm::onRemoveAll()
{
    // Возвращаем все из выбранных в доступные
    while (selectedList->count() > 0)
    {
        QListWidgetItem* item = selectedList->takeItem(0);
        availableList->addItem(item->text());
        delete item;
    }
    infoLabel->setText(QString("Выбрано %1 изделий").arg(selectedList->count()));
}

void EdgeForm::setInfo(const QString& info)
{
    infoEdit->setPlainText(info);
}

QString EdgeForm::getInfo() const
{
    return infoEdit->toPlainText().trimmed();
}

void EdgeForm::setProducts(const QList<QString>& products)
{
    selectedList->clear();
    selectedList->addItems(products);
    infoLabel->setText(QString("Выбрано %1 изделий").arg(selectedList->count()));


    // Очищаем от мусора.
    for (int i = selectedList->count() - 1; i >= 0; --i)
    {
        QListWidgetItem* item = selectedList->item(i);
        if (!allAvailableProducts.contains(item->text()))
            delete selectedList->takeItem(i);
    }

    // Удаляем выбранные из доступных
    for (int i = availableList->count() - 1; i >= 0; --i)
    {
        QListWidgetItem* item = availableList->item(i);
        if (products.contains(item->text()))
            delete availableList->takeItem(i);
    }
}

QList<QString> EdgeForm::getProducts() const
{
    QList<QString> products;
    for (int i = 0; i < selectedList->count(); ++i)
        products.append(selectedList->item(i)->text().trimmed());
    return products;
}

void EdgeForm::setAvailableProducts(const QList<QString>& products)
{
    availableList->clear();
    availableList->addItems(products);
    allAvailableProducts = products;
    // Удаляем уже выбранные
    for (int i = availableList->count() - 1; i >= 0; --i)
    {
        QListWidgetItem* item = availableList->item(i);
        // Проверяем, нет ли этого товара уже в выбранных
        bool found = false;
        for (int j = 0; j < selectedList->count(); ++j)
        {
            if (selectedList->item(j)->text() == item->text())
            {
                found = true;
                break;
            }
        }
        if (found)
            delete availableList->takeItem(i);
    }



}

void EdgeForm::setReadOnly(bool readOnly)
{
    infoEdit->setEnabled(!readOnly);
    selectedList->setEnabled(!readOnly);
    availableList->setEnabled(!readOnly);

    infoLabel->setVisible(!readOnly);
    addButton->setVisible(!readOnly);
    removeButton->setVisible(!readOnly);
    addAllButton->setVisible(!readOnly);
    removeAllButton->setVisible(!readOnly);
}
