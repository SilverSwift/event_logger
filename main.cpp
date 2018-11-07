#include <QApplication>

#include "mainwidget.h"

#include "eventplayer.h"
#include "eventrecorder.h"
#include "fileserializer.h"
#include "modemanager.h"
#include "testcontrol.h"
#include "widgetidcontroller.h"

#include <QtDebug>

int main(int argc, char** argv)
{

    QApplication app(argc, argv);
   
    MainWidget wgt;
    wgt.show();
    wgt.setObjectName("MainWindowTest");

    //parsing input arguments(mode and filename) for start program

    QStringList stringList = ModeManager::parse(qApp);

    if( stringList.isEmpty() ){
        return 1;
    }

    QString mode(stringList.at(0));
    QString fileName(stringList.at(1));

    EventRecorder recorder(new FileSerializer(fileName));

    EventPlayer player(new FileSerializer(fileName));

    TestControl testWidget;
    testWidget.show();
    testWidget.move(0,0);

    WidgetIdController controller;

    ModeManager modeManager(mode);

    QObject::connect(&testWidget, &TestControl::modeWasChanged,
                         &modeManager, &ModeManager::changeMode);

    QObject::connect(&testWidget, &TestControl::stop,
                         &modeManager, &ModeManager::stop);

    QObject::connect(&modeManager, &ModeManager::startRecord,
                         &recorder, &AbstractRecorder::start);

    QObject::connect(&modeManager, &ModeManager::stopRecord,
                         &recorder, &AbstractRecorder::stop);

    QObject::connect(&modeManager, &ModeManager::startPlay,
                         &player, &AbstractPlayer::start);

    QObject::connect(&modeManager, &ModeManager::stopPlay,
                         &player, &AbstractPlayer::stop);

    modeManager.start();

    /*QObject::connect(&testWidget, &TestControl::startRecord,
                     &recorder, &AbstractRecorder::start);

    QObject::connect(&testWidget, &TestControl::stopRecord,
                     &recorder, &AbstractRecorder::stop);

    QObject::connect(&testWidget, &TestControl::startPlay,
                     &player, &AbstractPlayer::start);

    QObject::connect(&testWidget, &TestControl::stopPlay,
                     &player, &AbstractPlayer::stop);*/

    return app.exec();
}
