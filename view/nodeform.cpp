#include "nodeform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

NodeForm::NodeForm(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Сведения о заводе");
    setModal(true);
    resize(400, 240);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Поле для имени
    QLabel* label = new QLabel("Название:", this);
    nameEdit = new QLineEdit(this);
    layout->addWidget(label);
    layout->addWidget(nameEdit);

    // Поле для описания
    label = new QLabel("Описание:", this);
    infoEdit = new QPlainTextEdit(this);
    layout->addWidget(label);
    layout->addWidget(infoEdit);

    // Кнопки
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okButton = new QPushButton("ОК", this);
    QPushButton* cancelButton = new QPushButton("Отмена", this);

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

void NodeForm::setInfo(const QString& info)
{
    infoEdit->setPlainText(info);
}

QString NodeForm::getInfo() const
{
    return infoEdit->toPlainText();
}
