/* Menu list refresh: picks the id of the current entry (7FFF when none), updates the display, notifies the manager and redraws. */
struct W {
    char p0[0x2558]; unsigned flags; char p1[0xC]; int mode; char p2[0x2694 - 0x2568 - 4]; int page; int cur;
};
struct Self { char p[0xA690]; W *w; };
extern "C" {
extern char *D_08B01CF8;
void func_08AE6400(Self *, int, int, int);
void func_08AF4E98(char *, int);
void func_08AE6CFC(Self *, int);
void func_08AE61A8(Self *self)
{
    int id = 0x7FFF;
    int flag = 0;
    int extra = 0;
    int idx;
    unsigned fl = self->w->flags;
    idx = self->w->cur;
    if (fl & 4) {
        flag = 1;
        id = *(unsigned short *)((char *)self->w + 0x7EC0 + idx * 0x57E + *(int *)((char *)self->w + 0x25C0) * 2);
    } else if (fl & 2) {
        if (self->w->mode != 1) {
            id = ((unsigned short *)((char *)self->w + 0x7E98 + self->w->page * 8))[idx];
            extra = 1;
        } else {
            flag = 1;
        }
    } else {
        id = ((unsigned short *)((char *)self->w + 0x7E98 + self->w->page * 8))[idx];
        extra = 1;
    }
    func_08AE6400(self, idx, id, extra);
    func_08AF4E98(D_08B01CF8, flag);
    func_08AE6CFC(self, 0);
}
}
