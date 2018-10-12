#include "testcontrol.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

#include <QDebug>

TestControl::TestControl(QWidget *parent) : QWidget(parent)
{
    /*pathLine = new QLineEdit("C:/sources/build-testProgramForEventLogger-Desktop-Debug/debug/testProgramForEventLogger.exe",
                             this);*/

    //QPushButton* browse = new QPushButton("browse", this);

    QHBoxLayout* browseRow = new QHBoxLayout();
    browseRow->setAlignment(Qt::AlignHCenter);
    //browseRow->addWidget(pathLine);
    //browseRow->addWidget(browse);

    /*connect(browse, &QPushButton::clicked,
            this, &TestControl::slotBrowse);*/

    QRadioButton* recordModeButton = new QRadioButton("Record");
    QRadioButton* playModeButton = new QRadioButton("Play");
    recordModeButton->setChecked(true);

    QButtonGroup* buttonGroup = new QButtonGroup();
    buttonGroup->addButton(recordModeButton);
    buttonGroup->addButton(playModeButton);
    buttonGroup->setId(recordModeButton, 1);
    buttonGroup->setId(playModeButton, 0);

    connect(buttonGroup,  QOverload<int>::of(&QButtonGroup::buttonClicked),
            this, &TestControl::changeMode);

    QVBoxLayout* radioButtons = new QVBoxLayout();

    radioButtons->addWidget(recordModeButton);
    radioButtons->addWidget(playModeButton);

    QPushButton* start = new QPushButton("start", this);
    QPushButton* stop = new QPushButton("stop", this);

    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->setAlignment(Qt::AlignHCenter);
    btnRow->addWidget(start);
    btnRow->addWidget(stop);

    QVBoxLayout* layout = new QVBoxLayout(this);
    //layout->addLayout(browseRow);
    layout->addLayout(radioButtons);
    layout->addLayout(btnRow);

    connect(start, &QPushButton::clicked,
            this, &TestControl::startClicked);

    connect(stop, &QPushButton::clicked,
            this, &TestControl::stopClicked);

}

void TestControl::changeMode(int button)
{
    this->recordMode = button;

    this->recordMode ? qDebug()<< "recordMode" : qDebug()<< "playMode";

}

/*void TestControl::slotBrowse()
{
    QString str = QFileDialog::getOpenFileName(nullptr,
                                               "Select a program",
                                               pathLine->text(),
                                               tr("Applications (*.exe)"));
    if (!str.isEmpty()) {
    pathLine->setText(str);

    path = str;
    }
}*/

void TestControl::startClicked()
{
    this->recordMode ? emit startRecord() : emit startPlay();
}
void TestControl::stopClicked()
{
    this->recordMode ?  emit stopRecord() : emit stopPlay();
}
