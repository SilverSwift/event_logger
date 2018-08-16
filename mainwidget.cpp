#include "mainwidget.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    QPushButton* inc = new QPushButton("increment");
    QPushButton* reset = new QPushButton("reset");
    QLabel* value = new QLabel("0");
    QComboBox* delta = new QComboBox();
    delta->insertItems(0, {"one", "two", "three", "four"});

    connect(inc, &QPushButton::clicked, this, [=](){
        int v = value->text().toInt();
        int d = delta->currentIndex() + 1;
        value->setText(QString::number(v + d));
    });

    connect(reset, &QPushButton::clicked,
            this, [=](){value->setText("0");});



    QHBoxLayout* btnRow = new QHBoxLayout();
    btnRow->setAlignment(Qt::AlignHCenter);
    btnRow->addWidget(inc);
    btnRow->addWidget(reset);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(value);
    layout->addWidget(delta);
    layout->addLayout(btnRow);
}


