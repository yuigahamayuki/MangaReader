#include "iconlabel.h"

IconLabel::IconLabel(QWidget *parent)
    : QLabel(parent)
{
    QImage iconImage("plus-circle.png");
    setPixmap(QPixmap::fromImage(iconImage));
    setContentsMargins(30,30,30,30);
}

void IconLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    emit clicked();
}
