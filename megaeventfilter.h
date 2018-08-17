#ifndef MEGAEVENTFILTER_H
#define MEGAEVENTFILTER_H

#include <QObject>
#include <QJsonArray>

class MegaEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit MegaEventFilter(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event) override;
    ~MegaEventFilter() override;

signals:

public slots:

private:
    QJsonArray jsonArray;
    void saveJsonFile();
    QJsonObject findParentObject(QObject* obj);
};

#endif // MEGAEVENTFILTER_H
