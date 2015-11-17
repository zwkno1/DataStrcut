#ifndef ControlWidget_H
#define ControlWidget_H

#include <QWidget>

namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = 0);
    ~ControlWidget();
    void insert(int v)
    {
        emit signalInsert(v);
    }

    void remove(int v)
    {
        emit signalRemove(v);
    }

signals:
    void signalInsert(int v);
    void signalRemove(int v);
private slots:
    void onInsert();
    void onRemove();

private:
    Ui::ControlWidget *ui;
};

#endif // ControlWidget_H
