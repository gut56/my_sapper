#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    Tile t;
//    t.show();
    Widget w;
    w.show();


    return a.exec();
}
