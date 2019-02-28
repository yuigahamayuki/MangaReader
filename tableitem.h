#ifndef TABLEITEM_H
#define TABLEITEM_H

#include <QWidget>
#include <QMessageBox>
#include <QGuiApplication>
#include <QVBoxLayout>
#include "imagelabel.h"
#include "textlabel.h"
#include <QScreen>

class TableItem : public QWidget
{
    Q_OBJECT
public:
    explicit TableItem(QWidget *parent = nullptr);
    bool setCoverImage(const QString &fileName);
    void setVolumeName(const QString &volumeName);
    ImageLabel* getImageLabel();

private:
    ImageLabel *imageLabel;
    TextLabel *textLabel;
    QVBoxLayout *layout;
};

#endif // TABLEITEM_H
