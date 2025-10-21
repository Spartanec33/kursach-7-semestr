#include "edgeform.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

EdgeForm::EdgeForm(QWidget* parent) : QDialog(parent)
{
    setWindowTitle("Edge Properties");
    setModal(true);
    resize(300, 120);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Поле для имени
    QLabel* label = new QLabel("Edge info:", this);
    infoEdit = new QLineEdit(this);
    layout->addWidget(label);
    layout->addWidget(infoEdit);

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

void EdgeForm::setInfo(const QString& info)
{
    infoEdit->setText(info);
}

QString EdgeForm::getInfo() const
{
    return infoEdit->text();
}
