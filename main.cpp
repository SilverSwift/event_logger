#include <QApplication>
#include <QPushButton>
#include <QtWidgets>

#include "mainwidget.h"

#include "eventplayer.h"
#include "eventrecorder.h"
#include "fileserializer.h"
#include "testcontrol.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    MainWidget wgt;
    wgt.show();
    wgt.setObjectName("MainWindowTest");

    EventRecorder recorder(new FileSerializer("output"));
    EventPlayer player(new FileSerializer("output"));

    TestControl testWidget;
    testWidget.show();

    QObject::connect(&testWidget, &TestControl::startClicked,
                     &recorder, &AbstractRecorder::start);

    QObject::connect(&testWidget, &TestControl::stopClicked,
                     &recorder, &AbstractRecorder::stop);

    return app.exec();
}
