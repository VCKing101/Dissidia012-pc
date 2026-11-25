/* Recreates the selected icon (when a row is chosen and its frame id is valid) and registers the widgets. */
extern void func_0887BA5C(void *h);
extern void *func_08AF39E8(void *g, int a, int b, int c, int d, int e);
extern void func_088D428C(void *a, void *b, const char *c, int d);
extern const char D_08B23F12[];
extern struct Tbl2308C { char pad[0x12]; unsigned short t[1]; } D_08B23F28;
extern void *D_08B01CF8;

typedef struct B2308C {
    char pad0[0x404];
    char at404[0x80];
    char pad484[0x884 - 0x484];
    char at884[0x18];
    void *h89C;
    char pad8A0[0x9A0 - 0x8A0];
    int sel;
    char pad9A4[0xC];
    int idx;
} B2308C;
typedef struct S2308C { char pad[0x138]; B2308C *blk; } S2308C;

void func_08B2308C(S2308C *self)
{
    B2308C *b = self->blk;
    int sel = b->sel;
    if (sel != -1) {
        unsigned short fr;
        unsigned short none;
        void *hh = b->h89C;
        char *zt = (char *)D_08B23F28.t;
        if (hh != 0) {
            func_0887BA5C(hh);
            self->blk->h89C = 0;
            b = self->blk;
            sel = b->sel;
        }
        { int so = sel << 5; char *row = (char *)(so + (int)zt); fr = *(unsigned short *)(row + b->idx * 2); }
        none = 0xFFFF;
        if (fr != none) {
            void *h = func_08AF39E8(D_08B01CF8, 3, 0, fr, 0, 0);
            self->blk->h89C = h;
            b = self->blk;
        }
    }
    func_088D428C(b->at404, b->at884, D_08B23F12, 1);
}
