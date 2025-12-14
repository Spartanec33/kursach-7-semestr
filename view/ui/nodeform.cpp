#include "nodeform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QInputDialog>
#include <QMessageBox>

NodeForm::NodeForm(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Название завода
    mainLayout->addWidget(new QLabel("Название завода:"));
    nameEdit = new QLineEdit;
    mainLayout->addWidget(nameEdit);

    // Описание завода
    mainLayout->addWidget(new QLabel("Описание:"));
    infoEdit = new QPlainTextEdit;
    infoEdit->setMaximumHeight(80);
    mainLayout->addWidget(infoEdit);

    // Разделитель
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    mainLayout->addWidget(line);

    // Список изделий
    mainLayout->addWidget(new QLabel("Производимые изделия:"));
    productsList = new QListWidget;
    mainLayout->addWidget(productsList);

    // Кнопки управления списком
    QHBoxLayout* buttonsLayout = new QHBoxLayout;

    addButton = new QPushButton("Добавить");
    editButton = new QPushButton("Изменить");
    removeButton = new QPushButton("Удалить");

    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(editButton);
    buttonsLayout->addWidget(removeButton);
    buttonsLayout->addStretch();

    mainLayout->addLayout(buttonsLayout);

    // Подключение сигналов
    connect(addButton, &QPushButton::clicked, this, &NodeForm::onAddProduct);
    connect(editButton, &QPushButton::clicked, this, &NodeForm::onEditProduct);
    connect(removeButton, &QPushButton::clicked, this, &NodeForm::onRemoveProduct);
}

void NodeForm::onAddProduct()
{
    bool ok;
    QString product = QInputDialog::getText(this, "Добавить изделие",
                                          "Название изделия:",
                                          QLineEdit::Normal, "", &ok);
    if (ok && !product.trimmed().isEmpty())
    {
        if (hasProduct(product))
        {
            QMessageBox::warning(this, "Предупреждение",
                "Такое изделие уже есть в списке!");
            return;
        }
        productsList->addItem(product.trimmed());
    }
}

void NodeForm::onEditProduct()
{
    QListWidgetItem* currentItem = productsList->currentItem();
    if (!currentItem)
    {
        QMessageBox::information(this, "Информация", "Выберите изделие для редактирования.");
        return;
    }

    bool ok;
    QString newProduct = QInputDialog::getText(this, "Изменить изделие",
                                             "Новое название:",
                                             QLineEdit::Normal,
                                             currentItem->text(), &ok);
    if (ok && !newProduct.trimmed().isEmpty())
    {
        if (hasProduct(newProduct, currentItem))
        {
            QMessageBox::warning(this, "Предупреждение",
               "Такое изделие уже есть в списке!");
            return;
        }
        currentItem->setText(newProduct.trimmed());
    }
}

void NodeForm::onRemoveProduct()
{
    QListWidgetItem* currentItem = productsList->currentItem();
    if (!currentItem)
        QMessageBox::information(this, "Информация", "Выберите изделие для удаления.");
    else
        delete currentItem;
}

bool NodeForm::hasProduct(const QString& product, QListWidgetItem* excludeItem)
{
    QString searchProduct = product.trimmed();

    for (int i = 0; i < productsList->count(); ++i)
    {
        QListWidgetItem* item = productsList->item(i);

        // Пропускаем исключенный элемент
        if (excludeItem && item == excludeItem)
            continue;

        if (item->text().trimmed().compare(searchProduct, Qt::CaseInsensitive) == 0)
            return true;
    }

    return false;
}

void NodeForm::setName(const QString& name)
{
    nameEdit->setText(name);
}

QString NodeForm::getName() const
{
    return nameEdit->text().trimmed();
}

void NodeForm::setInfo(const QString& info)
{
    infoEdit->setPlainText(info);
}

QString NodeForm::getInfo() const
{
    return infoEdit->toPlainText().trimmed();
}

void NodeForm::setProducts(const QList<QString>& products)
{
    productsList->clear();
    productsList->addItems(products);
}

QList<QString> NodeForm::getProducts() const
{
    QList<QString> products;
    for (int i = 0; i < productsList->count(); ++i)
        products.append(productsList->item(i)->text().trimmed());
    return products;
}

void NodeForm::setReadOnly(bool readOnly)
{
    nameEdit->setReadOnly(readOnly);
    infoEdit->setReadOnly(readOnly);
    productsList->setEnabled(!readOnly);

    addButton->setVisible(!readOnly);
    editButton->setVisible(!readOnly);
    removeButton->setVisible(!readOnly);
}
