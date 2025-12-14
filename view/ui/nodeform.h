#ifndef NODEFORM_H
#define NODEFORM_H

#include <QWidget>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>

class NodeForm : public QWidget
{
    Q_OBJECT
public:
    explicit NodeForm(QWidget* parent = nullptr);

    void setName(const QString& name);
    QString getName() const;

    void setInfo(const QString& info);
    QString getInfo() const;

    void setProducts(const QList<QString>& products);
    QList<QString> getProducts() const;

    void setReadOnly(bool readOnly);

private slots:
    void onAddProduct();
    void onEditProduct();
    void onRemoveProduct();

private:
    bool hasProduct(const QString& product, QListWidgetItem* excludeItem = nullptr);

    QLineEdit* nameEdit;
    QPlainTextEdit* infoEdit;
    QListWidget* productsList;
    QPushButton* addButton;
    QPushButton* editButton;
    QPushButton* removeButton;
};

#endif // NODEFORM_H
