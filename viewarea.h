#ifndef VIEWAREA_H
#define VIEWAREA_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>

class ViewArea : public QWidget
{
    Q_OBJECT
public:
    explicit ViewArea(QWidget *parent = nullptr);
    void setLeftImage(const QString& leftImageName);
    void setRightImage(const QString& rightImageName);
    void setCoverImage(const QString& coverImageName);
    QSize getImageSize();
private:
    QLabel *leftLabel, *rightLabel;
    QHBoxLayout *layout;
    QSize imageSize;

};

#endif // VIEWAREA_H
