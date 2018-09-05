#ifndef TESTCONTROL_H
#define TESTCONTROL_H

#include <QWidget>

class TestControl : public QWidget
{
    Q_OBJECT
public:
    explicit TestControl(QWidget *parent = nullptr);

signals:
    void startClicked();
    void stopClicked();

};

#endif // TESTCONTROL_H
