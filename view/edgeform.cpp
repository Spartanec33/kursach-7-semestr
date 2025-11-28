#include "edgeform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

EdgeForm::EdgeForm(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Сведения о поставке");
    setModal(true);
    resize(400, 240);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Поле для описания
    QLabel* label = new QLabel("Описание:", this);
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

void EdgeForm::setInfo(const QString& info)
{
    infoEdit->setPlainText(info);
}

QString EdgeForm::getInfo() const
{
    return infoEdit->toPlainText();
}
