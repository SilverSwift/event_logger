#include "modemanager.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QProcess>

ModeManager::ModeManager(QString mode, QObject *parent) :
    QObject(parent),
    mMode(mode)
{
}

void ModeManager::changeMode(bool inputMode)
{
    QString mode;
    inputMode ? mode = recordMode : mode = playMode;
    QProcess::startDetached(QApplication::applicationFilePath(), QStringList() << mode);

    QApplication::exit();
}

void ModeManager::start()
{

    if(mMode == recordMode)
    {
        emit startRecord();
    }
    else if(mMode == playMode)
    {
        emit startPlay();
    }

}
void ModeManager::stop()
{
    if(mMode == recordMode)
    {
        emit stopRecord();
    }
    else if(mMode == playMode)
    {
        emit stopPlay();
    }
}

ModeManager::~ModeManager()
{
}

QStringList ModeManager::validateArguments(QStringList args)
{
    QStringList resultStringList;

    QString defaultMode = noneMode;
    QString defaultFileName(QApplication::applicationDirPath() + "/output");

    switch( args.size() ){
        case 0:{
            resultStringList << defaultMode << defaultFileName;
            printf("%s\n%s\n%s\n", infoMessage, qPrintable(defaultMode), qPrintable(defaultFileName));

        }
        break;

        case 1:{
            QString inputMode = args.at(0);

            QFile defaultFile(defaultFileName);

            if(inputMode == noneMode || inputMode == recordMode || (inputMode == playMode && defaultFile.exists())){
                resultStringList << inputMode << defaultFileName;
                printf("%s\n%s\n%s\n", infoMessage, qPrintable(inputMode), qPrintable(defaultFileName));
            } else{
                fputs(modeError, stderr);
            }

        }
        break;

        case 2:{
            QString inputMode = args.at(0);
            QString inputPath = args.at(1);

            if(inputMode == noneMode || inputMode == recordMode || inputMode == playMode){
                QFile inputFile(inputPath);
                if(inputFile.exists() ||
                        (inputMode != playMode && QFileInfo(inputFile).absoluteDir().exists())) {
                    resultStringList << inputMode << inputPath;
                    printf("%s\n%s\n%s\n", infoMessage, qPrintable(inputMode), qPrintable(inputPath));
                }
                else {
                   fputs(fileNameError, stderr);
                }
            }
            else {
                fputs(modeError, stderr);
            }

        }
        break;

    }

    return resultStringList;

}

QStringList ModeManager::parse(QApplication* inputApp){

    QCommandLineParser parser;

    parser.addHelpOption();

    parser.addPositionalArgument("mode", QApplication::translate("main", "programMode"));

    parser.addPositionalArgument("file", QApplication::translate("main", "File for recording or playing events"));

    parser.process(*inputApp);

    const QStringList args = parser.positionalArguments();

    return ModeManager::validateArguments(args);
}
