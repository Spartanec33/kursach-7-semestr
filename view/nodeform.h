#ifndef NODEFORM_H
#define NODEFORM_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>

//Окно информации об узле
class NodeForm : public QDialog
{
    Q_OBJECT
public:
    NodeForm(QWidget* parent = nullptr);
    void setName(const QString& name);
    QString getName() const;
    void setInfo(const QString& Info);
    QString getInfo() const;

private:
    QLineEdit* nameEdit;
    QPlainTextEdit* infoEdit;
};

#endif // NODEFORM_H
