#include "nodeform.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QIntValidator>

NodeForm::NodeForm(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(new QLabel("Название узла:"));

    nameEdit = new QLineEdit;

    mainLayout->addWidget(nameEdit);

    // Прижимаем поле к верху, чтобы оно не растягивалось на всё окно
    mainLayout->addStretch();
}

void NodeForm::setName(const QString& name)
{
    nameEdit->setText(name);
}

QString NodeForm::getName() const
{
    return nameEdit->text().trimmed();
}

void NodeForm::setReadOnly(bool readOnly)
{
    nameEdit->setReadOnly(readOnly);
    nameEdit->setEnabled(!readOnly);
}
