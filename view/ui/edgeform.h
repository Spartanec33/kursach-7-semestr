#ifndef EDGEFORM_H
#define EDGEFORM_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>

class EdgeForm : public QWidget
{
    Q_OBJECT
public:
    explicit EdgeForm(QWidget* parent = nullptr);

    void setInfo(const QString& info);
    QString getInfo() const;

    void setProducts(const QList<QString>& products);  // Выбранные изделия
    QList<QString> getProducts() const;

    void setAvailableProducts(const QList<QString>& products);  // Доступные у отправителя
    void setReadOnly(bool readOnly);

private slots:
    void onAddProduct();
    void onRemoveProduct();
    void onAddAll();
    void onRemoveAll();

private:
    QList<QString> allAvailableProducts;

    QPlainTextEdit* infoEdit;
    QListWidget* availableList;
    QListWidget* selectedList;
    QPushButton* addButton;
    QPushButton* removeButton;
    QPushButton* addAllButton;
    QPushButton* removeAllButton;
    QLabel* infoLabel;
};

#endif // EDGEFORM_H
