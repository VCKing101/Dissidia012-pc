/* Fills the 120-entry level table from the item list, clamping ids and values. */
typedef struct Big { char pad[0x1DE04]; char tbl[0x78]; } Big;
typedef struct Item { char a; unsigned char id; unsigned char v; } Item;
extern Big *D_089D712C;
extern char D_089D5FD3;
extern void func_08935828(char *, int, int);
extern void func_088F45FC(void);
extern int func_088F47C4(void);
extern Item *func_088C50D4(int);
extern int func_088F4900(int, int, Item *, int);
void func_088F4638(char *t)
{
    unsigned int v;
    int id;
    int i;
    char *p;
    if (t == 0) {
        p = D_089D712C->tbl;
        func_08935828(p, 0, 0x78);
        func_088F45FC();
        D_089D5FD3 = 1;
    } else {
        p = t;
    }
    for (i = 0; i < func_088F47C4(); i++) {
        Item *it = func_088C50D4(i);
        int lim;
        id = it->id;
        if (id < 0 || id >= 0x78) id = 0;
        v = it->v & 0xF;
        lim = func_088F4900(0x40, 0, it, 0);
        if (lim <= (int)v) v = 0;
        *(char *)((int)p + id) = v;
    }
}
