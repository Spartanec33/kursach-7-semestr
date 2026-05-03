#ifndef EDGEFORM_H
#define EDGEFORM_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QString>

//Виджет параметров поставки
class EdgeForm : public QWidget
{
    Q_OBJECT
public:
    explicit EdgeForm(QWidget* parent = nullptr);

    void setInfo(const QString& info);                // Установить описание поставки
    QString getInfo() const;                          // Получить текст описания

    void setProducts(const QList<QString>& products); // Заполнить список выбранных товаров
    QList<QString> getProducts() const;               // Получить итоговый список поставки

    void setAvailableProducts(const QList<QString>& products); // Загрузить склад завода-отправителя
    void setReadOnly(bool readOnly);                  // Переключить форму в режим «только чтение»

private slots:
    void onAddProduct();      // Перенести выбранный товар в поставку (>)
    void onRemoveProduct();   // Вернуть товар на склад отправителя (<)
    void onAddAll();          // Перенести все доступные товары в поставку (>>)
    void onRemoveAll();       // Очистить список поставки (<<)

private:
    QList<QString> allAvailableProducts; // Полный перечень продукции отправителя

    QPlainTextEdit* infoEdit;     // Поле для ввода текстового описания
    QListWidget* availableList;   // Список «Доступно на заводе»
    QListWidget* selectedList;    // Список «В поставке»
    QPushButton* addButton;       // Кнопка переноса элемента
    QPushButton* removeButton;    // Кнопка возврата элемента
    QPushButton* addAllButton;    // Кнопка переноса всего списка
    QPushButton* removeAllButton; // Кнопка полной очистки
    QLabel* infoLabel;            // Информационная строка состояния
};

#endif // EDGEFORM_H
