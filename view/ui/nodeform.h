#ifndef NODEFORM_H
#define NODEFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class NodeForm : public QWidget
{
    Q_OBJECT
public:
    NodeForm(QWidget* parent = nullptr);

    void setName(const QString& name);
    QString getName() const;

    void setInfo(const QString& info);
    QString getInfo() const;

    void setReadOnly(bool readOnly); //Установить флаг "только для чтения"

private:
    QLineEdit* nameEdit;
    QPlainTextEdit* infoEdit;
};

#endif // NODEFORM_H
