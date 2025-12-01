#include "edgeform.h"
#include <QVBoxLayout>
#include <QLabel>

EdgeForm::EdgeForm(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* label = new QLabel("Описание:", this);
    infoEdit = new QPlainTextEdit(this);

    layout->addWidget(label);
    layout->addWidget(infoEdit);
}

void EdgeForm::setInfo(const QString& info)
{
    infoEdit->setPlainText(info);
}


QString EdgeForm::getInfo() const
{
    return infoEdit->toPlainText();
}

//Установить флаг "только для чтения"
void EdgeForm::setReadOnly(bool readOnly)
{
    infoEdit->setReadOnly(readOnly);
}
