#ifndef WIDGETIDCONTROLLER_H
#define WIDGETIDCONTROLLER_H

#include <QObject>

class WidgetIdController : public QObject
{
    Q_OBJECT
public:
    explicit WidgetIdController(QObject *parent = nullptr);
    bool eventFilter(QObject *watched, QEvent *event) override;
    static void startWidgetNamer();

protected:
    void inspect();
    void nameWidget(QWidget *widget);
    static bool sortByClassName(const QWidget* w1, const QWidget* w2);

private:
    int mCnt = 0;
};

#endif // WIDGETIDCONTROLLER_H
