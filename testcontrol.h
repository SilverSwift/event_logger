#ifndef TESTCONTROL_H
#define TESTCONTROL_H

#include <QLineEdit>
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
    //void slotBrowse();
private:
    bool recordMode = true;
    //QLineEdit* pathLine;
    //QString path = nullptr;
};

#endif // TESTCONTROL_H
