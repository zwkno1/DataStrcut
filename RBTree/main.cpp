#include <QApplication>
#include <random>
#include <thread>
#include "rbtree.h"
#include "showwidget.h"
#include "controlwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    rbtree<int> t;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 100000);

    ShowWidget w(t);
    ControlWidget control_widget;
    QObject::connect(&control_widget, &ControlWidget::signalInsert, &w, &ShowWidget::insert);
    QObject::connect(&control_widget, &ControlWidget::signalRemove, &w, &ShowWidget::remove);

    auto test = [&t, &dis, &gen, &control_widget]()
    {
        for(int i = 0; i != 10; ++i)
        {
            control_widget.insert(dis(gen));
        }
        for(int i = 0; i != 10; ++i)
        {
            control_widget.remove(dis(gen));
        }
    };
    std::thread thread(test);
    thread.join();
    control_widget.show();
    w.show();

    return a.exec();
}
