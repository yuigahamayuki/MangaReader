#include "mangareader.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MangaReader w;
    w.show();

    return a.exec();
}
