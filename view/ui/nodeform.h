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

    void setInfo(const QString& info);                // Установить текстовое описание завода
    QString getInfo() const;                          // Получить текст описания из редактора

    void setProducts(const QList<QString>& products); // Заполнить список выпускаемых изделий
    QList<QString> getProducts() const;               // Сформировать список изделий из элементов интерфейса

    void setReadOnly(bool readOnly);                  // Переключить виджет в режим «только чтение»

private slots:
    void onAddProduct();                              // Слот для добавления нового изделия в список
    void onEditProduct();                             // Слот для изменения названия выбранного изделия
    void onRemoveProduct();                           // Слот для удаления изделия из списка

private:
    // Проверка на дубликаты названий в списке (игнорируя регистр)
    bool hasProduct(const QString& product, QListWidgetItem* excludeItem = nullptr);

    QLineEdit* nameEdit;        // Однострочный редактор для названия завода
    QPlainTextEdit* infoEdit;   // Многострочный редактор для описания
    QListWidget* productsList;  // Графический список (виджет) продукции
    QPushButton* addButton;     // Кнопка вызова диалога добавления
    QPushButton* editButton;    // Кнопка вызова диалога редактирования
    QPushButton* removeButton;  // Кнопка удаления элемента списка
};

#endif // NODEFORM_H
