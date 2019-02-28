#ifndef TEXTLABEL_H
#define TEXTLABEL_H
#include <QLabel>

class TextLabel : public QLabel
{
    Q_OBJECT
public:
    TextLabel(QWidget *parent = 0);
protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void clicked();
};

#endif // TEXTLABEL_H
