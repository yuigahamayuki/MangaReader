#ifndef IMAGELABEL_H
#define IMAGELABEL_H
#include <QLabel>

class ImageLabel : public QLabel
{
    Q_OBJECT
public:
    ImageLabel(QWidget *parent = 0);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
signals:
    void clicked();
};

#endif // IMAGELABEL_H
