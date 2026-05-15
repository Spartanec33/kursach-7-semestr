#ifndef GRAPHCONTROLLER_H
#define GRAPHCONTROLLER_H

#include <QFile>
#include <QObject>
#include <QPointF>
#include "graph.h"
#include "graphview.h"
#include "nodeform.h"
#include "edgeform.h"
#include <QMessageBox>
#include <vector>

//Основной контроллер системы, связывающий графическое представление с объектами графа
class GraphController : public QObject
{
    Q_OBJECT
public:
    GraphController(QObject* parent = nullptr);

public slots:
    void addNodeAt(const QPointF& position); // Добавить узел в точку
    void addEdge(int fromId, int toId);      // Добавить ребро между узлами

    void removeSelectedNode(); // Удалить выбранный узел
    void removeSelectedEdge(); // Удалить выбранное ребро

    void clearGraph();   //Очистить граф
    void showInfoForm(); //Показать окно информации

    void saveGraph(); //Сохранить граф
    void loadGraph(); //Загрузить граф

    void graphChanged(); //Обработать изменение графа

    void undo();   //Шаг в прошлое в истории графов
    void redo();   //Шаг в будущее в истории графов

    bool handleCloseEvent(); //Обработать закрытие программы

    void showSolveDialog(); //Показать диалог для решения задачи ДП
    void solvePathWithExport(int startId, int endId); //Решить задачу поиска кратчайшего пути и экспортировать результат

    GraphView* getView(){return view;} // Получить указатель на графическое представление
    Graph* getGraph(){return graph;}   // Получить указатель на текущую модель графа
private:
    void removeNode(int id); // Удалить узел по его id
    void removeEdge(int id); // Удалить ребро по его id

    QDialog* createDialog(const QString& title, QWidget* content); // Общая функция создания диалога с кнопками
    void processNodeForm(int selectedNodeId);                      // Обработка информационного окошка узла
    void processEdgeForm(int selectedEdgeId);                      // Обработка информационного окошка ребра

    vector<Graph> graphHistory; // История графов
    int currentIndex = -1;      // Текущее положение в истории графов
    Graph* graph;    //Модель
    GraphView* view; //Вид

};
#endif // GRAPHCONTROLLER_H
