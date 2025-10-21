#ifndef NODEFORM_H
#define NODEFORM_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

//Окно информации об узле
class NodeForm : public QDialog
{
    Q_OBJECT
public:
    NodeForm(QWidget* parent = nullptr);
    void setName(const QString& name);
    QString getName() const;

private:
    QLineEdit* nameEdit;
};

#endif // NODEFORM_H
