#ifndef EDGEFORM_H
#define EDGEFORM_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

//Окно информации о ребре
class EdgeForm : public QDialog
{
    Q_OBJECT
public:
    EdgeForm(QWidget* parent = nullptr);

    void setInfo(const QString& Info);
    QString getInfo() const;

private:
    QLineEdit* infoEdit;
};

#endif // EDGEFORM_H
