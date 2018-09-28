#ifndef WIDGETNAMER_H
#define WIDGETNAMER_H

#include <QObject>

class WidgetNamer : public QObject
{
    Q_OBJECT
public:
    explicit WidgetNamer(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event) override;
    static void startWidgetNamer();

protected:
    void inspect();
    void nameWidget(QWidget* widget);

private:
    int mCnt = 0;
};

#endif // WIDGETNAMER_H
