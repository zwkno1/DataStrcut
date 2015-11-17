#include "showwidget.h"

ShowWidget::ShowWidget(rbtree<int> &t)
    : tree_(t)
{
    this->setMaximumSize(9999, 9999);

}

void ShowWidget::paintEvent(QPaintEvent *e)
{
    QPainter p(this);
    int x = 768;
    int y = 32;
    paint(p, x, y, 384, 32, tree_.root());
}

void ShowWidget::paint(QPainter &p, int x, int y, int dx, int dy, rbtree<int>::node *n)
{
    drawNode(p, x, y, n);
    if(tree_.is_not_nil(n))
    {
        paint(p, x - dx, y + dy, dx/2, dy, n->left);
        p.drawLine(x, y, x - dx, y + dy);
        paint(p, x + dx, y + dy, dx/2, dy, n->right);
        p.drawLine(x, y, x + dx, y + dy);
    }
}
