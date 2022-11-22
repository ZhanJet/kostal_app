#ifndef STATUSLIGHT_H
#define STATUSLIGHT_H
#include <QLabel>
#include <QString>

/**
 * @brief create a light blub to show assigned status
 * @param[in] label label for this light blub
 * @param[in] color color for this light blub
 * @param[in] size size for this light blub
 */
void setStatusLight(QLabel* label, int color, int size)
{
    // setText should be set with null string
    label->setText("");
    QString min_width = QString("min-width: %1px;").arg(size);
    QString min_height = QString("min-height: %1px;").arg(size);
    QString max_width = QString("max-width: %1px;").arg(size);
    QString max_height = QString("max-height: %1px;").arg(size);
    QString border_radius = QString("border-radius: %1px;").arg(size / 2);
    QString border = QString("border:1px solid black;");
    QString background = "background-color:";

    switch (color) {
        case 0:
            // grey
            background += "rgb(190,190,190)";
            break;
        case 1:
            // red
            background += "rgb(255,0,0)";
            break;
        case 2:
            // green
            background += "rgb(0,255,0)";
            break;
        case 3:
            // yellow
            background += "rgb(255,255,0)";
            break;
        default:
            break;
    }
    const QString SheetStyle = min_width + min_height + max_width + max_height
                               + border_radius + border + background;
    label->setStyleSheet(SheetStyle);
}

#endif // STATUSLIGHT_H
