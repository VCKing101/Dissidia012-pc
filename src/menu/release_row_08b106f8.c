typedef struct Row { int h[9]; } Row;
typedef struct Wk { char pad[0xA18]; int h[1]; } Wk;
typedef struct Sc { char pad[0x138]; Wk *wk; } Sc;
extern void func_088D3EA0(char *, char *, char *);
extern void func_088D436C(char *, char *, char *);
extern void func_0887BA5C(int);
extern void func_0887B944(int);
void func_08B106F8(Sc *self, int idx)
{
    Wk *w;
    int off;
    w = self->wk;
    func_088D3EA0((char *)w + 4, (char *)w + 0x404, (char *)w + idx * 0x40 + 0x1FA7C);
    w = self->wk;
    func_088D436C((char *)w + 0x404, (char *)w + 0x884, (char *)w + idx * 0x48 + 0x1FCFC);
    off = idx * 9;
    if (self->wk->h[off + 0] != 0) {
        func_0887BA5C(self->wk->h[off + 0]);
        self->wk->h[off + 0] = 0;
    }
    if (self->wk->h[off + 1] != 0) {
        func_0887BA5C(self->wk->h[off + 1]);
        self->wk->h[off + 1] = 0;
    }
    if (self->wk->h[off + 2] != 0) {
        func_0887BA5C(self->wk->h[off + 2]);
        self->wk->h[off + 2] = 0;
    }
    if (self->wk->h[off + 3] != 0) {
        func_0887BA5C(self->wk->h[off + 3]);
        self->wk->h[off + 3] = 0;
    }
    if (self->wk->h[off + 4] != 0) {
        func_0887BA5C(self->wk->h[off + 4]);
        self->wk->h[off + 4] = 0;
    }
    if (self->wk->h[off + 5] != 0) {
        func_0887BA5C(self->wk->h[off + 5]);
        self->wk->h[off + 5] = 0;
    }
    if (self->wk->h[off + 6] != 0) {
        func_0887BA5C(self->wk->h[off + 6]);
        self->wk->h[off + 6] = 0;
    }
    func_0887B944(self->wk->h[off + 7]);
    func_0887B944(self->wk->h[off + 8]);
}
