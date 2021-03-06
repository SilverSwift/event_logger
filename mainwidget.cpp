#include "mainwidget.h"

#include <QApplication>
#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QPushButton>
#include <QVBoxLayout>

MainWidget::MainWidget(QWidget *parent) : QWidget(parent)
{
    QMenuBar *menuBar = new QMenuBar(this);



    QMenu* pmnu = new QMenu("&Menu", menuBar);

    pmnu->addAction("&About Qt",
                    qApp,
                    &QApplication::aboutQt,
                    QKeySequence::HelpContents);

    pmnu->addSeparator();

    QAction* pCheckableAction = pmnu->addAction("&CheckableItem");
    pCheckableAction->setCheckable(true);
    pCheckableAction->setChecked(true);

    menuBar->addMenu(pmnu);

    QPushButton* inc = new QPushButton("increment");
    QPushButton* reset = new QPushButton("reset");
    QLabel* value = new QLabel("0");
    QComboBox* delta = new QComboBox();
    delta->insertItems(0, {"one", "two", "three", "four"});
    QLineEdit* lineEdit = new QLineEdit;

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
    layout->addWidget(menuBar);
    layout->addWidget(value);
    layout->addWidget(delta);
    layout->addLayout(btnRow);
    layout->addWidget(lineEdit);
}


