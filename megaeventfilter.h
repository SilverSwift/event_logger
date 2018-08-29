#ifndef MEGAEVENTFILTER_H
#define MEGAEVENTFILTER_H

#include <QObject>
#include <QJsonArray>
#include <QApplication>
#include <QSet>

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
    QSet<QString> objectNames;
    void saveJsonFile();
    QString findParentObject(QObject* obj);
    void setWidgetName(); //setObjectName
    QString createWidgetName(QString str);
    void handleWidgetName(QWidget* w);
};

#endif // MEGAEVENTFILTER_H
