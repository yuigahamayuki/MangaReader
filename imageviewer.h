#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QMainWindow>
#include "viewarea.h"
#include <QScrollArea>
#include <QGuiApplication>
#include <QScreen>
#include <QSettings>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "iconlabel.h"
#include <QComboBox>

class ImageViewer : public QMainWindow
{
    Q_OBJECT
public:
    ImageViewer(QWidget *parent = nullptr, QSettings* _settings = nullptr, int _volumeIndex = 0);
    ViewArea* getViewArea();

private:
    ViewArea *viewArea;
    QScrollArea *scrollArea;
    QSettings *settings;
    int volumeIndex; //当前书在QSettings的index
    QString dirName;
    int volumePageCnt;
    double scaleFactor = 1.0;
    int currentPageNum = 0; //当前页数，初始为第0页，即封面，每次执行setPrevious/NextPages，这个值一般+2，除非到达末页。一般为偶数
    int chapterCnt = 0; //章节数，若没有设置过章节，则为0   
    QComboBox *chapterTraverseBox;
    QComboBox *chapterInfoBox;
    int currentChapter = 0;

    QAction *fitToWindowAct;

    void createMenus();   
    void createToolBars();
    void scaleImage(double factor);
    void originalSize();
    void initializeSetChaptersDialog(QDialog *dialog, QGridLayout *layout, IconLabel *addChapterIcon, QPushButton *confirmButton, QPushButton *cancelButton);

signals:

private slots:
    void setPreviousPages();
    void setNextPages();
    void setChapters();
    void fitToWindow();
    void addChapterIconOnClicked(QDialog *dialog, QGridLayout *layout, QPushButton *confirmButton, QPushButton *cancelButton);
    void changeCurrentChapter(int chapterIndex);
    void changeCurrentPage(int pageIndex);
};


#endif // IMAGEVIEWER_H
