#ifndef MEGAEVENTFILTER_H
#define MEGAEVENTFILTER_H

#include <QObject>
#include <QJsonArray>
#include <QApplication>

class MegaEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit MegaEventFilter(QObject *parent = nullptr);
    ~MegaEventFilter() override;

    bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void inspect();

protected slots:
    void onInspect();
    void logInputEvent(QObject *watched, QEvent *event);

private:
    QJsonArray jsonArray;
    int mCnt = 0;

    QJsonObject logEntry(QObject *watched, QEvent *event);
    QString findParentObject(QObject* obj);
    void nameWidget(QWidget* w);
    void saveJsonFile();


};

#endif // MEGAEVENTFILTER_H
