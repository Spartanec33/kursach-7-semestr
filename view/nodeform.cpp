#include "nodeform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

NodeForm::NodeForm(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Node Properties");
    setModal(true);
    resize(300, 120);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Поле для имени
    QLabel* label = new QLabel("Node name:", this);
    nameEdit = new QLineEdit(this);
    layout->addWidget(label);
    layout->addWidget(nameEdit);

    // Кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("OK", this);
    QPushButton* cancelButton = new QPushButton("Cancel", this);

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    layout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void NodeForm::setName(const QString& name)
{
    nameEdit->setText(name);
}

QString NodeForm::getName() const
{
    return nameEdit->text();
}
