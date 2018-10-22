#include <QApplication>
#include <QPushButton>
#include <QtWidgets>

#include "mainwidget.h"

#include "eventplayer.h"
#include "eventrecorder.h"
#include "fileserializer.h"
#include "testcontrol.h"
#include "widgetidcontroller.h"

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

    WidgetIdController controller;

    QObject::connect(&testWidget, &TestControl::startRecord,
                     &recorder, &AbstractRecorder::start);

    QObject::connect(&testWidget, &TestControl::stopRecord,
                     &recorder, &AbstractRecorder::stop);

    QObject::connect(&testWidget, &TestControl::startPlay,
                     &player, &AbstractPlayer::start);

    QObject::connect(&testWidget, &TestControl::stopPlay,
                     &player, &AbstractPlayer::stop);

    return app.exec();
}
