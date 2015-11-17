#ifndef RBTREE_H
#define RBTREE_H

#include <cassert>

template<class _Ty>
class rbtree
{
    friend class ShowWidget;
    typedef _Ty value_type;
    enum node_color
    {
        RED = 0,
        BLACK,
    };

    struct node
    {
        node(node *p = 0, const value_type & v = value_type(), node *l = 0, node *r = 0, char c = BLACK)
                : left(l)
                , parent(p)
                , right(r)
                , value(v)
                , color(c)
        {
        }

        node *left;
        node *parent;
        node *right;
        value_type value;
        //bit 0 : color(0:red, 1:black);
        //bit 1 : (0:not nil, 1:nil);
        char color;
    };

public:
    rbtree()
    {
        nil_ = allocate_node();
        if(nil_ == nullptr)
            throw std::bad_alloc();
        set_nil(nil_);
        root_ = nil_;
        parent(root_) = nil_;
    }

    void left_rotate(node *x)
    {
        /*
        *         x                         y
        *        / \                       / \
        *       a   y        ==>          x   c
        *	        / \                   / \
        *	       b   c                 a   b
        */
        node *p = parent(x);
        node *y = right(x);

        parent(y) = p;
        if(x == left(p))
            left(p) = y;
        else
            right(p) = y;

        parent(left(y)) = x;
        right(x) = left(y);

        parent(x) = y;
        left(y) = x;
        if(x == root())
            root() = y;
    }

    void right_rotate(node *x)
    {
        /*
        *         x                      y
        *        / \                    / \
        *       y   c        ==>       a   x
        *      / \                        / \
        *	   a   b                      b   c
        */
        node *p = parent(x);
        node *y = left(x);

        parent(y) = p;
        if(x == left(p))
            left(p) = y;
        else
            right(p) = y;

        parent(right(y)) = x;
        left(x) = right(y);

        parent(x) = y;
        right(y) = x;
        if(x == root())
            root() = y;
    }

    void insert(const _Ty & v)
    {
        node *p = nil();
        node *n = root();
        while (is_not_nil(n))
        {
            p = n;
            if(v < value(n))
                n = left(n);
            else if(value(n) < v)
                n = right(n);
            else
                return;
        }

        node *x = allocate_node();
        parent(x) = p;
        value(x) = v;
        left(x) = nil();
        right(x) = nil();
        set_red(x);

        if(is_nil(p))
        {
            root() = x;
            set_black(x);
            return;
        }
        if(v < value(p))
            left(p) = x;
        else
            right(p) = x;

        insert_fixup(x);
    }

    void insert_fixup(node * x)
    {
        node *p;
        node *pp;
        node *sp;
        while(is_not_root(x) && is_red(parent(x)))
        {
            p = parent(x);
            pp = parent(p);
            if(p == left(pp))
            {
                sp = right(pp);
                if(is_red(sp))
                {
                    set_black(p);
                    set_black(sp);
                    set_red(pp);
                    x = pp;
                }
                else
                {
                    if(x == right(p))
                    {
                        left_rotate(p);
                        p = x;
                    }
                    right_rotate(pp);
                    set_red(pp);
                    set_black(p);
                    x = p;
                    break;
                }
            }
            else
            {
                sp = left(pp);
                if(is_red(sp))
                {
                    set_black(p);
                    set_black(sp);
                    set_red(pp);
                    x = pp;
                }
                else
                {
                    if(x == left(p))
                    {
                        right_rotate(p);
                        p = x;
                    }
                    left_rotate(pp);
                    set_red(pp);
                    set_black(p);
                    x = p;
                    break;
                }
            }
        }
        if(is_root(x))
            set_black(x);
    }

    void remove(const _Ty & v)
    {
        node *n = root();
        while(n != nil())
        {
            if(v < value(n))
                n = left(n);
            else if(value(n) < v)
                n = right(n);
            else
                break;
        }

        if(is_not_nil(n))
            remove(n);
    }

    node * find_min(node *x)
    {
        node * n = x;
        while(is_not_nil(x))
        {
            n = x;
            x = left(x);
        }
        return n;
    }

    node * successor(node * x)
    {
        if(is_not_nil(right(x)))
            return find_min(right(x));
        assert(0);
        node * p = parent(x);
        while (is_not_nil(p) && x == right(p)) {
            x = p;
            p = parent(p);
        }
        return p;
    }

    void remove(node * x)
    {
        node * y = x;
        if(is_not_nil(left(x)) && is_not_nil(right(x)))
            y = successor(x);

        node * z;
        if(is_not_nil(left(y)))
            z = left(y);
        else
            z = right(y);

        parent(z) = parent(y);
        if(is_nil(parent(y)))
            root() = z;
        else if(y == left(parent(y)))
            left(parent(y)) = z;
        else
            right(parent(y)) = z;

        bool fix = is_black(y);
        if(y != x)
        {
            parent(y) = parent(x);
            if(x == left(parent(x)))
                left(parent(x)) = y;
            else
                right(parent(x)) = y;
            if(is_root(x))
                root() = y;

            left(y) = left(x);
            right(y) = right(x);

            parent(left(x)) = y;
            parent(right(x)) = y;

            if(is_red(x))
                set_red(y);
            else
                set_black(y);
        }

        if(fix)
            remove_fixup(z);
        deallocate_node(x);
    }

    void remove_fixup(node * x)
    {
        node * sx;
        node * px;
        while (is_not_root(x) && is_black(x))
        {
            px = parent(x);
            if(x == left(px))
            {
                // 1
                sx = right(px);
                if(is_red(sx))
                {
                    set_red(px);
                    set_black(sx);
                    left_rotate(px);
                    sx = right(px);
                }

                // 2
                if(is_black(left(sx)) && is_black(right(sx)))
                {
                    set_red(sx);
                    x = px;
                }
                else
                {
                    // 3
                    if(is_black(right(sx)))
                    {
                        set_red(sx);
                        set_black(left(sx));
                        right_rotate(sx);
                        sx = parent(sx);
                    }

                    // 4
                    if(is_red(px))
                    {
                        set_red(sx);
                        set_black(px);
                    }
                    else
                        set_black(sx);
                    set_black(right(sx));
                    left_rotate(px);
                    x = root();
                }
            }
            else
            {
                // 1
                sx = left(px);
                if(is_red(sx))
                {
                    set_red(px);
                    set_black(sx);
                    right_rotate(px);
                    sx = left(px);
                }

                // 2
                if(is_black(left(sx)) && is_black(right(sx)))
                {
                    set_red(sx);
                    x = px;
                }
                else
                {
                    // 3
                    if(is_black(left(sx)))
                    {
                        set_red(sx);
                        set_black(right(sx));
                        left_rotate(sx);
                        sx = parent(sx);
                    }

                    // 4
                    if(is_red(px))
                    {
                        set_red(sx);
                        set_black(px);
                    }
                    else
                        set_black(sx);
                    set_black(left(sx));
                    right_rotate(px);
                    x = root();
                }
            }
        }
        set_black(x);
    }

private:
    node *allocate_node() { return new node(); }
    void deallocate_node(node * x) { assert(x != nullptr); delete x; }

    bool is_nil(node * x) { assert(x != nullptr); return x->color & 0x02; }
    bool is_not_nil(node * x) { return !is_nil(x); }
    bool is_root(node * x) { assert(x != nullptr); return x == root(); }
    bool is_not_root(node * x) { return !is_root(x); }
    bool is_black(node * x) { assert(x != nullptr); return x->color & 0x01; }
    bool is_red(node * x) { return !is_black(x); }
    node *& parent(node * x) { assert(x != nullptr); return x->parent; }
    node *& left(node * x) { assert(x != nullptr); return x->left; }
    node *& right(node * x) { assert(x != nullptr); return x->right; }
    value_type & value(node * x) { assert(x != nullptr); return x->value; }
    char color(node * x) { assert(x != nullptr); return x->color & 0x01; }
    node *& root() { assert(root_ != nullptr); return root_; }
    node * nil() { assert(root_ != nullptr); return nil_; }

    void set_red(node * x) { assert(x != nullptr); x->color &= 0xFE; }
    void set_black(node * x) { assert(x != nullptr); x->color |= 0x01; }
    void set_nil(node * x) { assert(x != nullptr); x->color |= 0x03; }

private:
    node *root_;
    node *nil_;
};

#endif // RBTREE_H
