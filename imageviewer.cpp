#include "imageviewer.h"

ImageViewer::ImageViewer(QWidget *parent, QSettings *_settings,int _volumeIndex)
    : QMainWindow(parent),
      viewArea(new ViewArea(this)),
      scrollArea(new QScrollArea(this)),
      settings(_settings),
      volumeIndex(_volumeIndex)
{
    settings->beginGroup(QString("Volume%1").arg(volumeIndex));
    QString title = settings->value("VolumeName").toString();
    dirName = settings->value("DirName").toString();
    volumePageCnt = settings->value("VolumePageCnt").toInt();
    chapterCnt = settings->value("ChapterCnt").toInt();
    settings->endGroup();
    getViewArea()->setCoverImage(dirName + "/000.jpg");
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidget(viewArea);

    setCentralWidget(scrollArea);

    createMenus();
    createToolBars();

    fitToWindowAct->setEnabled(true);

    setWindowTitle(title);
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

void ImageViewer::createMenus()
{
    QMenu *editMenu = menuBar()->addMenu("Edit");
    editMenu->addAction("Set Chapters", this, &ImageViewer::setChapters);

    QMenu *viewMenu = menuBar()->addMenu("View");
    fitToWindowAct = viewMenu->addAction("Fit to Window", this, &ImageViewer::fitToWindow);
    fitToWindowAct->setShortcut(tr("Ctrl+F"));
    fitToWindowAct->setEnabled(false);
    fitToWindowAct->setCheckable(true);
}

void ImageViewer::createToolBars()
{
    QToolBar *manipulatePageToolBar = addToolBar("ManipulatePage");
    manipulatePageToolBar->addAction("Previous Pages", this, &ImageViewer::setPreviousPages);
    manipulatePageToolBar->addAction("Next Pages", this, &ImageViewer::setNextPages);

    QToolBar *chapterTraverseToolBar = addToolBar("ChapterTraverse");
    chapterTraverseBox = new QComboBox(this);
    for (int i = 0; i < chapterCnt; ++i)
    {
        chapterTraverseBox->addItem(QString("Chapter%1").arg(i));
    }
    chapterTraverseToolBar->addWidget(chapterTraverseBox);
    chapterInfoBox = new QComboBox(this);
    chapterTraverseToolBar->addWidget(chapterInfoBox);
    connect(chapterTraverseBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){changeCurrentChapter(index);});
    connect(chapterInfoBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [=](int index){changeCurrentPage(index);});
}

ViewArea* ImageViewer::getViewArea()
{
    return viewArea;
}

void ImageViewer::setPreviousPages()
{
    if (currentPageNum == 0)
    {
        return;
    }

    if (currentPageNum % 2 == 0)
    {
        --currentPageNum;
        --currentPageNum;
    }
    else {
        --currentPageNum;
    }
    viewArea->setLeftImage(dirName + QString("/%1.jpg").arg(currentPageNum, 3, 10, QChar('0')));
    if (currentPageNum == 0) //左页为002和右页为001的情况，只需设置左页为封面
    {
        chapterInfoBox->setCurrentIndex(0);
        return;
    }

    viewArea->setRightImage(dirName + QString("/%1.jpg").arg(currentPageNum-1, 3, 10, QChar('0')));

    int currentChapterStartPageNum = 0;

    while(currentChapter >= 0)
    {
        settings->beginGroup(QString("Volume%1").arg(volumeIndex));
        currentChapterStartPageNum = settings->value(QString("Chapter%1").arg(currentChapter)).toInt();
        settings->endGroup();
        if (currentPageNum >= currentChapterStartPageNum)
        {
            break;
        }

        --currentChapter;
    }
    int oldCurrentPageNum = currentPageNum;
    chapterTraverseBox->setCurrentIndex(currentChapter); //currentIndexChanged event会调用，pageInfoBox也会变，变为第一页,currentPageNum会变化
    currentPageNum = oldCurrentPageNum;
    chapterInfoBox->setCurrentIndex(currentPageNum - currentChapterStartPageNum);
}

void ImageViewer::setNextPages()
{
    if (currentPageNum == volumePageCnt - 1)
        return;
    if (currentPageNum + 2 == volumePageCnt) //总页数为偶数的情况，最后一面只有一页
    {
        ++currentPageNum;
        viewArea->setRightImage(dirName + QString("/%1.jpg").arg(currentPageNum, 3, 10, QChar('0')));
    }
    else
    {
        ++currentPageNum;
        viewArea->setRightImage(dirName + QString("/%1.jpg").arg(currentPageNum, 3, 10, QChar('0')));
        ++currentPageNum;
        viewArea->setLeftImage(dirName + QString("/%1.jpg").arg(currentPageNum, 3, 10, QChar('0')));
    }

    viewArea->adjustSize(); //必须要

    int currentChapterStartPageNum = 0;
    int nextChapterStartPageNum = 0;
    for ( ; currentChapter < chapterCnt; ++currentChapter)
    {
        settings->beginGroup(QString("Volume%1").arg(volumeIndex));
        currentChapterStartPageNum = settings->value(QString("Chapter%1").arg(currentChapter)).toInt();
        if (currentChapter == chapterCnt - 1)
        {
            settings->endGroup();
            break;
        }
        nextChapterStartPageNum = settings->value(QString("Chapter%1").arg(currentChapter+1)).toInt();
        settings->endGroup(); //必须加
        if (currentPageNum >= currentChapterStartPageNum && currentPageNum < nextChapterStartPageNum)
        {
            break;
        }
    }

    chapterTraverseBox->setCurrentIndex(currentChapter); //currentIndexChanged event会调用，pageInfoBox也会变，变为第一页
    chapterInfoBox->setCurrentIndex(currentPageNum - currentChapterStartPageNum);
}

void ImageViewer::originalSize() //以图片原始大小显示
{
    QSize imageSize = viewArea->getImageSize();
    viewArea->setFixedSize(2 * imageSize.width(), imageSize.height());
}

void ImageViewer::fitToWindow() //以不需要滚动条方式显示图片
{
    bool isFitted = fitToWindowAct->isChecked();
    QSize imageSize = viewArea->getImageSize();
    int goodHeight = QGuiApplication::primaryScreen()->availableSize().height() - 60;
    viewArea->setFixedHeight(goodHeight);
    viewArea->setFixedWidth(goodHeight * imageSize.width() / imageSize.height() * 2);
    if (!isFitted)
        originalSize();
}

void ImageViewer::initializeSetChaptersDialog(QDialog *dialog, QGridLayout *layout, IconLabel *addChapterIcon, QPushButton *confirmButton, QPushButton *cancelButton)
{
    for (int i = 0; i < chapterCnt; ++i)
    {
        settings->beginGroup(QString("Volume%1").arg(volumeIndex));
        const QString& string = QString("Chapter%1").arg(i);
        layout->addWidget(new QLabel(string, dialog), i, 0);
        layout->addWidget(new QLineEdit(settings->value(string).toString(), dialog), i, 1);
        settings->endGroup();
    }

    layout->addWidget(addChapterIcon, chapterCnt, 0);
    layout->addWidget(confirmButton, chapterCnt+1, 0);
    layout->addWidget(cancelButton, chapterCnt+1, 1);

    dialog->setLayout(layout);
    dialog->resize(400, 200);

    connect(confirmButton, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(reject()));
}

void ImageViewer::setChapters()
{
    QDialog *setChaptersDialog = new QDialog(nullptr);
    setChaptersDialog->setFixedWidth(120);
    QGridLayout *layout = new QGridLayout;
    IconLabel *addChapterIcon = new IconLabel(setChaptersDialog);
    QPixmap iconImage = addChapterIcon->pixmap()->scaledToHeight(40);
    addChapterIcon->setPixmap(iconImage);
    QPushButton *confirmButton = new QPushButton(setChaptersDialog);
    QPushButton *cancelButton = new QPushButton(setChaptersDialog);
    confirmButton->setText("确定");
    cancelButton->setText("取消");
    confirmButton->setFixedSize(50, 40);
    cancelButton->setFixedSize(50, 40);
    connect(addChapterIcon, &IconLabel::clicked, this, [=] {addChapterIconOnClicked(setChaptersDialog, layout, confirmButton, cancelButton);});
    initializeSetChaptersDialog(setChaptersDialog, layout, addChapterIcon, confirmButton, cancelButton);
    if (setChaptersDialog->exec() == QDialog::Accepted)
    {
        chapterTraverseBox->clear();
        for(int i = 0; i < chapterCnt; ++i)
        {
            QLineEdit *lineEdit = qobject_cast<QLineEdit*>(layout->itemAtPosition(i,1)->widget());
            settings->beginGroup(QString("Volume%1").arg(volumeIndex));
            settings->setValue(QString("Chapter%1").arg(i), lineEdit->text());
            settings->endGroup();
            chapterTraverseBox->addItem(QString("Chapter%1").arg(i));
        }
        settings->beginGroup(QString("Volume%1").arg(volumeIndex));
        settings->setValue("ChapterCnt", chapterCnt);
        settings->endGroup();

    }
}

void ImageViewer::addChapterIconOnClicked(QDialog *dialog, QGridLayout *layout, QPushButton *confirmButton, QPushButton *cancelButton)
{
    IconLabel *addChapterIcon = qobject_cast<IconLabel*>(sender());
    layout->addWidget(new QLabel(QString("Chapter%1").arg(chapterCnt), dialog), chapterCnt, 0);
    layout->addWidget(new QLineEdit(dialog), chapterCnt, 1);
    chapterCnt++;
    layout->addWidget(addChapterIcon, chapterCnt, 0);

    layout->addWidget(confirmButton, chapterCnt+1, 0);
    layout->addWidget(cancelButton, chapterCnt+1, 1);

    dialog->setLayout(layout);
}

void ImageViewer::changeCurrentChapter(int chapterIndex) //改变chapterInfoDisplayLabel,上下翻页也会调用
{
    //分是否为最后章，最后一章用totalPageCnt计算章节页数
    int chapterPageCnt = 0;
    chapterInfoBox->clear();
    if (chapterIndex == chapterCnt - 1) //最后一章
    {
        settings->beginGroup(QString("Volume%1").arg(volumeIndex));
        chapterPageCnt = volumePageCnt - settings->value(QString("Chapter%1").arg(chapterIndex)).toInt();
        settings->endGroup();
    }
    else {
        settings->beginGroup(QString("Volume%1").arg(volumeIndex));
        chapterPageCnt = settings->value(QString("Chapter%1").arg(chapterIndex+1)).toInt() - settings->value(QString("Chapter%1").arg(chapterIndex)).toInt();
        settings->endGroup();
    }

    for (int i = 1; i <= chapterPageCnt; ++i)
    {
        chapterInfoBox->addItem(QString("%1/%2").arg(i).arg(chapterPageCnt));
    }

    //changeCurrentPage(0);
    chapterInfoBox->setCurrentIndex(0);
}

void ImageViewer::changeCurrentPage(int pageIndex)
{
    //QString currentChapter = chapterTraverseBox->currentText();
    currentChapter = chapterTraverseBox->currentIndex();
    settings->beginGroup(QString("Volume%1").arg(volumeIndex));
    //currentPageNum = settings->value(currentChapter).toInt() + pageIndex;
    currentPageNum = settings->value(QString("Chapter%1").arg(currentChapter)).toInt() + pageIndex;
    settings->endGroup();

    if (currentPageNum == 0)
    {
        viewArea->setLeftImage(dirName + QString("/%1.jpg").arg(currentPageNum, 3, 10, QChar('0')));
        viewArea->setRightImage(QString());
        viewArea->adjustSize(); //必须要
        return;
    }
    if (currentPageNum % 2 == 1 && currentPageNum == volumePageCnt - 1)
    {
        viewArea->setRightImage(dirName + QString("/%1.jpg").arg(currentPageNum, 3, 10, QChar('0')));
        viewArea->adjustSize(); //必须要
        return;
    }
    if(currentPageNum % 2 == 1 )
    {
        viewArea->setRightImage(dirName + QString("/%1.jpg").arg(currentPageNum, 3, 10, QChar('0')));
        currentPageNum++;
        viewArea->setLeftImage(dirName + QString("/%1.jpg").arg(currentPageNum, 3, 10, QChar('0')));
        viewArea->adjustSize(); //必须要
        return;
    }
    else
    {
        viewArea->setLeftImage(dirName + QString("/%1.jpg").arg(currentPageNum, 3, 10, QChar('0')));
        viewArea->setRightImage(dirName + QString("/%1.jpg").arg(currentPageNum - 1, 3, 10, QChar('0')));
        viewArea->adjustSize(); //必须要
        return;
    }
}
