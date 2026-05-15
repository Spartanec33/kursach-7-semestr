#include "edgeform.h"
#include <QVBoxLayout>
#include <QIntValidator>

EdgeForm::EdgeForm(QWidget* parent) : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(new QLabel("Вес связи (число):"));

    weightEdit = new QLineEdit;

    // Ограничиваем ввод: только целые числа от 0 до 1 000 000
    // Это автоматически запретит вводить буквы и знак "минус"
    QIntValidator* validator = new QIntValidator(0, 1000000, this);
    weightEdit->setValidator(validator);

    weightEdit->setPlaceholderText("Введите вес...");

    mainLayout->addWidget(weightEdit);
    mainLayout->addStretch();
}

void EdgeForm::setWeight(int weight)
{
    // Если пришло отрицательное число из кода — принудительно ставим 0
    weightEdit->setText(QString::number(qMax(0, weight)));
}

int EdgeForm::getWeight() const
{
    // Преобразуем текст обратно в число
    return weightEdit->text().toInt();
}

void EdgeForm::setReadOnly(bool readOnly)
{
    weightEdit->setReadOnly(readOnly);
    // Визуально серым закрасим, если только чтение
    weightEdit->setEnabled(!readOnly);
}
