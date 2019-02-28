#include "imagelabel.h"

ImageLabel::ImageLabel(QWidget *parent)
    : QLabel(parent)
{

}

void ImageLabel::mouseDoubleClickEvent(QMouseEvent *event)
{
    emit clicked();
}
