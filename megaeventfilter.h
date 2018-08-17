#ifndef MEGAEVENTFILTER_H
#define MEGAEVENTFILTER_H

#include <QObject>
#include <QJsonArray>

class MegaEventFilter : public QObject
{
    Q_OBJECT
private:
    QJsonArray jsonArray;
    void saveJsonFile();
    QJsonObject findParentObject(QObject* obj);
public:
    explicit MegaEventFilter(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event) override;
    ~MegaEventFilter() override;

signals:

public slots:
};

#endif // MEGAEVENTFILTER_H
