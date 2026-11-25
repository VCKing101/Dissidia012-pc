struct Node {
    char pad[0x14];
    short a;
    char pad2[4];
    short b;
};

struct Rec {
    int kind;
    int pad;
    Node *node;
};

struct Cmp {
    static short value(Rec *r)
    {
        short v = 0;
        switch (r->kind) {
        case 0:
            v = r->node->a;
            break;
        case 1:
            v = r->node->b;
            break;
        default:
            break;
        }
        return v;
    }
    static bool less(Rec *a, Rec *b)
    {
        short va = value(a);
        short vb = value(b);
        return vb < va;
    }
};

extern "C" {

Rec *func_08AF0F14(Rec *a, Rec *b, Rec *c)
{
    if (Cmp::less(a, b)) {
        if (Cmp::less(b, c)) {
            return b;
        }
        if (Cmp::less(a, c)) {
            return c;
        }
        return a;
    }
    if (Cmp::less(a, c)) {
        return a;
    }
    if (Cmp::less(b, c)) {
        return c;
    }
    return b;
}

}
