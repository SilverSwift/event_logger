#include <QApplication>

#include "mainwidget.h"
#include "megaeventfilter.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MainWidget wgt;
    wgt.show();

    MegaEventFilter filter;

    return app.exec();
}
