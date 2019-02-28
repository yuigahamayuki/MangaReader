#ifndef MANGAREADER_H
#define MANGAREADER_H

#include <QMainWindow>
#include <QTableWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QHeaderView>
#include <QSettings>
#include "tableitem.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QImageReader>
#include "iconlabel.h"
#include <QLineEdit>
#include <QPushButton>
#include <QTextCodec>
#include "imageviewer.h"

class MangaReader : public QMainWindow
{
    Q_OBJECT

public:
    MangaReader(QWidget *parent = 0);
    ~MangaReader();

private:
    QTableWidget *tableWidget;
    int volumeCnt = 0;
    QSettings *settings;
    IconLabel *addItem;  // plus-icon, idicate adding one volume
    const int columnCnt = 4;
    int rowCnt = 0;
    int itemCnt = 0;

private slots:
    void addVolume(); //点击'+' icon新增一部漫画
    void loadVolume(int volumeIndex);//双击一部漫画的封面，打开漫画, volumeIndex指示要载入的漫画在设置文件中的index
};

void initializeImageFileDialog(QFileDialog &dialog);
void initializeSetVolumeNameDialog(QDialog &dialog, QLineEdit *lineEdit);

#endif // MANGAREADER_H
