#ifndef EDGEFORM_H
#define EDGEFORM_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>

class EdgeForm : public QWidget
{
    Q_OBJECT
public:
    EdgeForm(QWidget* parent = nullptr);

    void setInfo(const QString& info);
    QString getInfo() const;
    void setReadOnly(bool readOnly); //Установить флаг "только для чтения"

private:
    QPlainTextEdit* infoEdit;
};

#endif // EDGEFORM_H
