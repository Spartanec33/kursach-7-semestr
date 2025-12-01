#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>
#include <QColor>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>

class Config
{
public:
    static Config& instance()
    {
        static Config instance;
        return instance;
    }

    Config() {load();}

    void load()
    {
        QSettings settings("config.ini", QSettings::IniFormat);

        // Настройки узлов
        m_nodeSize = settings.value("Node/nodeSize", 40).toInt();
        m_nodeColor = QColor(settings.value("Node/nodeColor", "gray").toString());
        m_selectedNodeColor = QColor(settings.value("Node/selectedNodeColor", "yellow").toString());
        m_borderWidth = settings.value("Node/borderWidth", 2).toInt();
        m_borderColor = QColor(settings.value("Node/borderColor", "black").toString());
        m_textSize = settings.value("Node/textSize", 10).toInt();
        m_nodeTextColor = QColor(settings.value("Node/nodeTextColor", "black").toString());

        // Настройки ребер
        m_edgeWidth = settings.value("Edge/edgeWidth", 3).toInt();
        m_edgeColor = QColor(settings.value("Edge/edgeColor", "black").toString());
        m_selectedEdgeColor = QColor(settings.value("Edge/selectedEdgeColor", "red").toString());
        m_edgeOffset = settings.value("Edge/edgeOffset", 10).toDouble();
        m_arrowSize = settings.value("Edge/arrowSize", 20).toInt();
        m_arrowColor = QColor(settings.value("Edge/arrowColor", "red").toString());
        m_selectedArrowColor = QColor(settings.value("Edge/selectedArrowColor", "green").toString());
        m_minDistanceToFind = settings.value("Edge/minDistanceToFind", 10).toDouble();
    }

    int m_nodeSize = 40;                    // размер узла
    QColor m_nodeColor = Qt::gray;          // цвет узла
    QColor m_selectedNodeColor = Qt::yellow;// цвет выделенного узла
    int m_borderWidth = 2;                  // ширина границ
    QColor m_borderColor = Qt::black;       // цвет границ
    int m_textSize = 10;                    // размер шрифта
    QColor m_nodeTextColor = Qt::black;     // цвет текста

    int m_edgeWidth = 3;                    // ширина ребра
    QColor m_edgeColor = Qt::black;         // цвет ребра
    QColor m_selectedEdgeColor = Qt::red;   // цвет выделенного ребра
    double m_edgeOffset = 10;               // смещение ребер
    int m_arrowSize = 20;                   // размер стрелки
    QColor m_arrowColor = Qt::red;          // цвет стрелки
    QColor m_selectedArrowColor = Qt::green;// цвет выделенной стрелки
    double m_minDistanceToFind = 10;        // минимальное расстояние для поиска ребра при клике
};

//Глобальные функции для взятия значений
inline int nodeSize() { return Config::instance().m_nodeSize; }
inline QColor nodeColor() { return Config::instance().m_nodeColor; }
inline QColor selectedNodeColor() { return Config::instance().m_selectedNodeColor; }
inline int borderWidth() { return Config::instance().m_borderWidth; }
inline QColor borderColor() { return Config::instance().m_borderColor; }

inline int edgeWidth() { return Config::instance().m_edgeWidth; }
inline QColor edgeColor() { return Config::instance().m_edgeColor; }
inline QColor selectedEdgeColor() { return Config::instance().m_selectedEdgeColor; }
inline int textSize() { return Config::instance().m_textSize; }
inline QColor nodeTextColor() { return Config::instance().m_nodeTextColor; }
inline double edgeOffset() { return Config::instance().m_edgeOffset; }
inline int arrowSize() { return Config::instance().m_arrowSize; }
inline QColor arrowColor() { return Config::instance().m_arrowColor; }
inline QColor selectedArrowColor() { return Config::instance().m_selectedArrowColor; }
inline double minDistanceToFind() { return Config::instance().m_minDistanceToFind; }

#endif // CONFIG_H
