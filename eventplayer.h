#ifndef EVENTPLAYER_H
#define EVENTPLAYER_H

#include "abstractplayer.h"
#include <QJsonArray>
#include <QJsonObject>

class EventPlayer : public AbstractPlayer
{
    Q_OBJECT
public:
    explicit EventPlayer(AbstractSerializer* serializer,
                           QObject *parent = nullptr);
    virtual ~EventPlayer() override;

public slots:
    void start() override;
    void stop() override;

protected:
    void handleEventArray();
//    QString findParentObject(QObject* obj);
    void inspect();
    QWidget* findByPropertyName(QString name, QString parents);
    void keyPress(QWidget* wgt, int key, Qt::KeyboardModifiers modifiers = Qt::NoModifier);
    void mousePress(QWidget* pwgt,
                    int x,
                    int y,
                    Qt::MouseButton bt = Qt::LeftButton,
                    Qt::MouseButtons bts = Qt::LeftButton);
//    QJsonObject logEntry(QObject *watched, QEvent *event);
//    void logInputEvent(QObject *watched, QEvent *event);
    void nameWidget(QWidget* widget);

protected slots:
    void generateEvent(QJsonObject obj);

private:
    QJsonArray jsonArray;
    int mCnt = 0;
};

#endif // EVENTPLAYER_H
