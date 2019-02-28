#include "textlabel.h"

TextLabel::TextLabel(QWidget *parent)
    :QLabel(parent)
{

}

void TextLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit clicked();
}
