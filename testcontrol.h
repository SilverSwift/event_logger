#ifndef TESTCONTROL_H
#define TESTCONTROL_H

#include <QWidget>

class TestControl : public QWidget
{
    Q_OBJECT
public:
    explicit TestControl(QWidget *parent = nullptr);

signals:
    void startRecord();
    void startPlay();
    void stopRecord();
    void stopPlay();
public slots:
    void changeMode(int);
    void startClicked();
    void stopClicked();
private:
    bool recordMode = true;
};

#endif // TESTCONTROL_H
