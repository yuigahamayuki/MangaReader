#include "tableitem.h"

TableItem::TableItem(QWidget *parent) :
    QWidget(parent),
    imageLabel(new ImageLabel(this)),
    textLabel(new TextLabel(this)),
    layout(new QVBoxLayout)
{
    imageLabel->setScaledContents(true);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    textLabel->setMaximumHeight(40); //必须要
    textLabel->setMaximumWidth(QGuiApplication::primaryScreen()->availableSize().width() * 0.25);
    textLabel->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    textLabel->setFont(QFont("Microsoft YaHei", 15));
    textLabel->adjustSize();

    layout->addWidget(imageLabel);
    layout->addWidget(textLabel);

    setLayout(layout);
}


bool TableItem::setCoverImage(const QString &fileName)
{
    QImage image;
    if(!image.load(fileName))
    {
        QMessageBox::information(this, QGuiApplication::applicationDisplayName(), QString("Can not load %1. Please check if the filename is the correct format 'xxx.jpg'.").arg(fileName));
        return false;
    }

    imageLabel->setPixmap(QPixmap::fromImage(image));
    //imageLabel->resize(0.25 * image.width(),0.25 * image.height()); //封面图尺寸自适应屏幕分辨率
    //imageLabel->setContentsMargins(40, 40 * image.height() / image.width(), 40, 40 * image.height() / image.width()); //必须要
    int marginWidth = 40 * QGuiApplication::primaryScreen()->availableSize().width() / 1920; //40的边缘是以1920宽度的分辨率为基准的
    int coverWidth = QGuiApplication::primaryScreen()->availableSize().width() * 0.25 - 2 * marginWidth;
    imageLabel->setFixedSize(coverWidth, coverWidth * image.height() / image.width());
    imageLabel->setContentsMargins(marginWidth, marginWidth * image.height() / image.width(), marginWidth, marginWidth * image.height() / image.width()); //必须要

    return true;
}

void TableItem::setVolumeName(const QString &volumeName)
{
    textLabel->setText(volumeName);
}

ImageLabel* TableItem::getImageLabel()
{
    return imageLabel;
}
