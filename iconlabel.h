#ifndef ICONLABEL_H
#define ICONLABEL_H

#include <QLabel>
#include <QWidget>

class IconLabel : public QLabel
{
    Q_OBJECT
public:
    explicit IconLabel(QWidget *parent = 0);
protected:
    virtual void mouseReleaseEvent(QMouseEvent * ev);
signals:
    void clicked();
};

#endif // ICONLABEL_H
