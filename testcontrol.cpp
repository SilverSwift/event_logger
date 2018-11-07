#include "testcontrol.h"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

TestControl::TestControl(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout* browseRow = new QHBoxLayout();
    browseRow->setAlignment(Qt::AlignHCenter);

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

}

void TestControl::startClicked()
{
    emit modeWasChanged(recordMode);
}
void TestControl::stopClicked()
{
    emit stop();
}


/*void TestControl::startClicked()
{
    this->recordMode ? emit startRecord() : emit startPlay();
}
void TestControl::stopClicked()
{
    this->recordMode ?  emit stopRecord() : emit stopPlay();
}*/
