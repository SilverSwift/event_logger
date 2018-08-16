#ifndef MEGAEVENTFILTER_H
#define MEGAEVENTFILTER_H

#include <QObject>

class MegaEventFilter : public QObject
{
    Q_OBJECT
public:
    explicit MegaEventFilter(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event) override;

signals:

public slots:
};

#endif // MEGAEVENTFILTER_H
