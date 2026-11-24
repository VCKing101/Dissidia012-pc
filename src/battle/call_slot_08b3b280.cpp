// Picks the handler object (+0x12C table, by state +0x108, or +0x110 when the state is 2 or 3) and calls its virtual slot 13 with the owner.
struct Owner57;
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
    virtual void v13(Owner57 *);
};
struct Owner57 {
    char pad0[0x108];
    int state;
    char pad1[4];
    int alt;
    char pad2[0x12C - 0x114];
    Handler57 *table[1];
};
extern "C" void func_08B3B280(Owner57 *self)
{
    int st = self->state;
    Handler57 **tab = self->table;
    if (st == 2 || st == 3) {
        tab[self->alt]->v13(self);
    } else {
        tab[st]->v13(self);
    }
}
