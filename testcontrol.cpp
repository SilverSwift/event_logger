#include "testcontrol.h"
#include <QHBoxLayout>
#include <QPushButton>

TestControl::TestControl(QWidget *parent) : QWidget(parent)
{
    QPushButton* start = new QPushButton("start", this);
    QPushButton* stop = new QPushButton("stop", this);

    QHBoxLayout* btnRow = new QHBoxLayout(this);
    btnRow->setAlignment(Qt::AlignHCenter);
    btnRow->addWidget(start);
    btnRow->addWidget(stop);

    connect(start, &QPushButton::clicked,
            this, &TestControl::startClicked);

    connect(stop, &QPushButton::clicked,
            this, &TestControl::stopClicked);

}
