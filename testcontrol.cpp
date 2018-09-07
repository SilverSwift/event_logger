#include "testcontrol.h"
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

TestControl::TestControl(QWidget *parent) : QWidget(parent)
{
    QRadioButton* recordMode = new QRadioButton("Record");
    QRadioButton* playMode = new QRadioButton("Play");
    recordMode->setChecked(true);

    QButtonGroup* buttonGroup = new QButtonGroup();
    buttonGroup->addButton(recordMode);
    buttonGroup->addButton(playMode);

    connect(buttonGroup,  QOverload<QAbstractButton *>::of(&QButtonGroup::buttonClicked),
            this, &TestControl::changeMode);

    QVBoxLayout* radioButtons = new QVBoxLayout();
    radioButtons->addWidget(recordMode);
    radioButtons->addWidget(playMode);

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

void TestControl::changeMode(int)
{

}

void TestControl::startClicked()
{
    if(recordMode.)
    {
        emit
    }
}
void TestControl::stopClicked()
{

}
