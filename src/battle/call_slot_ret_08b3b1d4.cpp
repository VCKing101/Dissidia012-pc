// Picks the handler object (+0x12C table, by state +0x108, or +0x110 when the state is 2 or 3), calls its virtual slot 13 and returns the result only when it is a type-6 object.
struct Owner57;
struct Typed57 {
    char pad0[0x542];
    unsigned char type;
    static bool is6(Typed57 *t) { return t->type == 6; }
};
struct Handler57 {
    virtual void v0(Owner57 *);
    virtual void v1(Owner57 *);
    virtual void v2(Owner57 *);
    virtual void v3(Owner57 *);
    virtual void v4(Owner57 *);
    virtual void v5(Owner57 *);
    virtual void v6(Owner57 *);
    virtual void v7(Owner57 *);
    virtual void v8(Owner57 *);
    virtual void v9(Owner57 *);
    virtual void v10(Owner57 *);
    virtual void v11(Owner57 *);
    virtual void v12(Owner57 *);
    virtual Owner57 *v13(Owner57 *);
};
struct Owner57 {
    char pad0[0x108];
    int state;
    char pad1[4];
    int alt;
    char pad2[0x12C - 0x114];
    Handler57 *table[1];
    static bool is6(Owner57 *o) { return Typed57::is6((Typed57 *)o); }
};
extern "C" Owner57 *func_08B3B1D4(Owner57 *self)
{
    Owner57 *r;
    int st = self->state;
    Handler57 **tab = self->table;
    if (st == 2 || st == 3) {
        r = tab[self->alt]->v13(self);
    } else {
        r = tab[st]->v13(self);
    }
    if (r != 0) {
        if (!Owner57::is6(r)) {
            int zero = 0;
            r = (Owner57 *)zero;
        }
    }
    return r;
}
