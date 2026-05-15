#ifndef NODEFORM_H
#define NODEFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

//Виджет параметров завода
class NodeForm : public QWidget
{
    Q_OBJECT
public:
    explicit NodeForm(QWidget* parent = nullptr);

    void setName(const QString& name);                // Установить название завода в поле ввода
    QString getName() const;                          // Получить введенное название завода
    void setReadOnly(bool readOnly);                  // Переключить виджет в режим «только чтение»


private:
    QLineEdit* nameEdit;        // Однострочный редактор для названия завода
};

#endif // NODEFORM_H
