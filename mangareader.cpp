#include "mangareader.h"

MangaReader::MangaReader(QWidget *parent)
    : QMainWindow(parent),
      tableWidget(new QTableWidget(this)),
      settings(new QSettings("settings.ini", QSettings::IniFormat)),
      addItem(new IconLabel)
{
    settings->setIniCodec(QTextCodec::codecForName("UTF-8"));

    tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableWidget->horizontalHeader()->setDefaultSectionSize(0.24 * QGuiApplication::primaryScreen()->availableSize().width()); //cell的width
    tableWidget->verticalHeader()->setDefaultSectionSize(0.24 * QGuiApplication::primaryScreen()->availableSize().width() * 1.5);//cell的height，高/宽比跟每张图一样，即1.5
    tableWidget->setShowGrid(false);

    connect(addItem, SIGNAL(clicked()), this, SLOT(addVolume()));

    volumeCnt = settings->value("VolumeCnt", 0).toInt();
    tableWidget->setColumnCount(columnCnt);
    for(int i = 0; i < ceil(double(volumeCnt+1) / double(columnCnt)); ++i)
    {
        tableWidget->insertRow(rowCnt);
        ++rowCnt;
    }
    for(int i = 1; i <= volumeCnt; ++i)
    {
        TableItem *volumeItem = new TableItem(this);
        settings->beginGroup(QString("Volume%1").arg(i));
        QString dirName = settings->value("DirName").toString();
        volumeItem->setCoverImage(dirName.append("/000.jpg"));
        volumeItem->setVolumeName(settings->value("VolumeName").toString());
        settings->endGroup();
        tableWidget->setCellWidget((i-1)/columnCnt, (i-1)%columnCnt, volumeItem);
        connect(volumeItem->getImageLabel(), &ImageLabel::clicked, this, [=] {loadVolume(i);});
    }
    int marginWidth = 40 * QGuiApplication::primaryScreen()->availableSize().width() / 1920;
    QPixmap iconImage = addItem->pixmap()->scaledToHeight(QGuiApplication::primaryScreen()->availableSize().width() * 0.25 - 2 * marginWidth);
    addItem->setPixmap(iconImage);
    tableWidget->setCellWidget(volumeCnt / columnCnt ,volumeCnt % columnCnt,addItem);
    itemCnt = volumeCnt + 1; //1表示"加icon"

    setCentralWidget(tableWidget);
    resize(QGuiApplication::primaryScreen()->availableSize() * 3 / 5);
}

MangaReader::~MangaReader()
{

}

void initializeImageFileDialog(QFileDialog &dialog)
{
    static bool firstDialog = true;

    if (firstDialog)
    {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    dialog.setFileMode(QFileDialog::Directory);
}

void MangaReader::addVolume()
{
    QFileDialog dialog(this, "Open Directory");
    initializeImageFileDialog(dialog);
    if (dialog.exec() == QDialog::Accepted)
    {
        itemCnt++;
        if(itemCnt == rowCnt * columnCnt)
        {
            tableWidget->insertRow(rowCnt);
            ++rowCnt;
        }
        QDialog setVolumeNameDialog(nullptr);
        QLineEdit *lineEdit = new QLineEdit(&setVolumeNameDialog);
        initializeSetVolumeNameDialog(setVolumeNameDialog, lineEdit);
        QString volumeName;
        QDir volumeDir(dialog.selectedFiles().first());
        volumeDir.setFilter(QDir::Files); //需设置这个filter，否则count的结果是实际图片数加2，推测多的是.和..
        if (setVolumeNameDialog.exec() == QDialog::Accepted)
        {
            volumeName = lineEdit->text();
        }
        else
        {
            volumeName = volumeDir.dirName();
        }

        TableItem *volumeItem = new TableItem(this);

        if (volumeItem->setCoverImage(QString(volumeDir.path()).append("/000.jpg"))) //成功读取封面图
        {
            volumeItem->setVolumeName(volumeName);
            tableWidget->setCellWidget(volumeCnt / columnCnt, volumeCnt % columnCnt, volumeItem); //在原先的+位置处插入新书
            volumeCnt++;
            addItem = new IconLabel; //setCellWidget(or removeCellWidget) delets old widget
            int marginWidth = 40 * QGuiApplication::primaryScreen()->availableSize().width() / 1920;
            QPixmap iconImage = addItem->pixmap()->scaledToHeight(QGuiApplication::primaryScreen()->availableSize().width() * 0.25 - 2 * marginWidth);
            addItem->setPixmap(iconImage);
            connect(addItem, SIGNAL(clicked()), this, SLOT(addVolume()));
            tableWidget->setCellWidget(volumeCnt / columnCnt, volumeCnt % columnCnt, addItem); //在原先的+的位置的下一个位置放入'+'icon

            settings->setValue("VolumeCnt", volumeCnt);
            settings->beginGroup(QString("Volume%1").arg(volumeCnt));
            settings->setValue("VolumeName", volumeName);
            settings->setValue("DirName", volumeDir.path());
            settings->setValue("VolumePageCnt", volumeDir.count());
            settings->setValue("ChapterCnt", 1);
            settings->setValue("Chapter0", 0);
            settings->endGroup();

            connect(volumeItem->getImageLabel(), &ImageLabel::clicked, this, [=] {loadVolume(volumeCnt);}); //SLOT函数如果带参数，需要使用lambda，信号不能使用SIGNAL(clicked())形式，why? 重载?
        }
    }
}

void initializeSetVolumeNameDialog(QDialog &dialog, QLineEdit *lineEdit)
{
    QLabel *textLabel = new QLabel("请输入书名：", &dialog);
    QPushButton *confirmButton = new QPushButton(&dialog);
    QPushButton *cancelButton = new QPushButton(&dialog);
    confirmButton->setText("确定");
    cancelButton->setText("取消");

    textLabel->setFont(QFont("Microsoft YaHei", 30, QFont::Bold));
    textLabel->setAlignment(Qt::AlignBottom);
    lineEdit->setMinimumHeight(80);
    lineEdit->setFont(QFont("Microsoft YaHei", 20, QFont::Bold));
    confirmButton->setMinimumHeight(40);
    cancelButton->setMinimumHeight(40);

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(textLabel, 0, 0, 1, 3);
    layout->addWidget(lineEdit, 1, 0, 1, 3);
    layout->addWidget(confirmButton, 2, 1);
    layout->addWidget(cancelButton, 2, 2);

    dialog.setLayout(layout);
    dialog.resize(400, 200);

    QObject::connect(confirmButton, SIGNAL(clicked()), &dialog, SLOT(accept()));
    QObject::connect(cancelButton, SIGNAL(clicked()), &dialog, SLOT(reject()));
}

void MangaReader::loadVolume(int volumeIndex)
{
    ImageViewer *imageViewer = new ImageViewer(nullptr, settings, volumeIndex);
    imageViewer->showMaximized();
    //imageViewer->activateWindow();
}

