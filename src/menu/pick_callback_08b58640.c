/* Picks one of three callbacks by mode into the record's slot at 0x2B98, then calls the registrar. */
struct Rec { char pad[0x2B98]; void *cb; };
struct Mgr { char pad[0x14]; struct Rec *rec; };
extern struct Mgr *D_089D7260;
extern void func_08B58AD4(void);
extern void func_08B58B00(void);
extern void func_08B58B14(void);
extern void func_088AF020(struct Mgr *, int);
void func_08B58640(int a, int mode) {
    struct Rec *r = D_089D7260->rec;
    switch (mode) {
    case 0: r->cb = func_08B58AD4; break;
    case 1: r->cb = func_08B58B00; break;
    case 2: r->cb = func_08B58B14; break;
    }
    func_088AF020(D_089D7260, a);
}
