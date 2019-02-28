#include "viewarea.h"

ViewArea::ViewArea(QWidget *parent)
    : QWidget(parent),
      leftLabel(new QLabel(this)),
      rightLabel(new QLabel(this)),
      layout(new QHBoxLayout)
{   
    //leftLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    leftLabel->setScaledContents(true);
    //rightLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    rightLabel->setScaledContents(true);
    leftLabel->setAlignment(Qt::AlignRight | Qt::AlignHCenter);//为了让左右图片靠在一起

    layout->setSpacing(0);//为了让左右图片靠在一起
    layout->addWidget(leftLabel);
    layout->addWidget(rightLabel);

    setLayout(layout);
}

void ViewArea::setLeftImage(const QString &leftImageName)
{
    QImage leftImage(leftImageName);
    leftLabel->setPixmap(QPixmap::fromImage(leftImage));
}

void ViewArea::setRightImage(const QString &rightImageName)
{
    QImage rightImage(rightImageName);
    rightLabel->setPixmap(QPixmap::fromImage(rightImage));
}

void ViewArea::setCoverImage(const QString &coverImageName)
{
    QImage coverImage(coverImageName);
    leftLabel->setPixmap(QPixmap::fromImage(coverImage));

    imageSize = coverImage.size();
    //setFixedSize(2 * coverImage.width(), coverImage.height()); //使得整个双页ViewArea这个Widget的尺寸不会因为窗口大小的改变而改变 *改用adjustSize()
}

QSize ViewArea::getImageSize()
{
    return imageSize;
}

