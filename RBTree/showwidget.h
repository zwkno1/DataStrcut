#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QtWidgets/QWidget>
#include <QPainter>
#include "rbtree.h"

class ShowWidget : public QWidget
{
public:
    ShowWidget(rbtree<int> &t);
public slots:
    void insert(int v)
    {
        tree_.insert(v);
        repaint();
    }

    void remove(int v)
    {
        tree_.remove(v);
        repaint();
    }

protected:
    void paintEvent(QPaintEvent *e);
private:
    void paint(QPainter & p, int x, int y, int dx, int dy, rbtree<int>::node * n);
    void drawNode(QPainter & p, int x, int y, rbtree<int>::node * n)
    {
        p.setPen((n->color & 0x01) ? Qt::black : Qt::red);
        //p.drawEllipse(x, y, x+r, y+r);
        p.drawText(x, y, (n->color & 0x02) ? QString("nil") : QString("").number(n->value, 10));
    }

    rbtree<int> & tree_;
    const static int r = 10;
};

#endif // SHOWWIDGET_H
