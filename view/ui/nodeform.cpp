#include "nodeform.h"
#include <QVBoxLayout>
#include <QLabel>

NodeForm::NodeForm(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Поле для имени
    QLabel* nameLabel = new QLabel("Название:", this);
    nameEdit = new QLineEdit(this);

    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);

    // Поле для описания
    QLabel* infoLabel = new QLabel("Описание:", this);
    infoEdit = new QPlainTextEdit(this);

    layout->addWidget(infoLabel);
    layout->addWidget(infoEdit);
}

void NodeForm::setName(const QString& name)
{
    nameEdit->setText(name);
}

QString NodeForm::getName() const
{
    return nameEdit->text();
}

void NodeForm::setInfo(const QString& info)
{
    infoEdit->setPlainText(info);
}

QString NodeForm::getInfo() const
{
    return infoEdit->toPlainText();
}

void NodeForm::setReadOnly(bool readOnly) //Установить флаг "только для чтения"
{
    nameEdit->setReadOnly(readOnly);
    infoEdit->setReadOnly(readOnly);
}
