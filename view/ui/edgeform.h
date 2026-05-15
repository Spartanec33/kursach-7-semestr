#ifndef EDGEFORM_H
#define EDGEFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>

class EdgeForm : public QWidget
{
    Q_OBJECT
public:
    explicit EdgeForm(QWidget* parent = nullptr);

    void setWeight(int weight);
    int getWeight() const;
    void setReadOnly(bool readOnly);

private:
    QLineEdit* weightEdit;
};

#endif // EDGEFORM_H
