#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include "eventconstants.h"

#include <QApplication>

#include <QObject>

class ModeManager : public QObject
{
    Q_OBJECT
public:
    ModeManager(QString mode, QObject *parent = nullptr);
    static QStringList parse(QApplication* inputApp);
    void start();
    static QStringList validateArguments(QStringList args);
    ~ModeManager();
signals:
    void startRecord();
    void stopRecord();

    void startPlay();
    void stopPlay();

public slots:
    void changeMode(bool);
    void stop();
private:
    QString mMode;
};

#endif // MODEMANAGER_H
