/* Menu list refresh: picks the id of the current entry (7FFF when none), updates the display and notifies the manager. */
struct W {
    char p0[0x1B7C]; unsigned flags; char p1[0x18]; int mode; char p2[0x28]; int cur; char p3[0x28]; int pos; char p4[0xB0]; int page;
    char p5[0x3064 - 0x1CA8]; unsigned short tab[1];
};
struct Self { char p[0x46AC]; W *w; };
extern "C" {
extern char *D_08B01CF8;
void func_08ADBFB8(Self *, int, int);
void func_08AF4E98(char *, int);
void func_08ADB9A0(Self *);
void func_08ADB830(Self *self)
{
    int id = 0x7FFF;
    int flag = 0;
    int idx;
    unsigned fl = self->w->flags;
    idx = self->w->cur;
    if (fl & 4) {
        flag = 1;
        id = *(unsigned short *)((char *)self->w + 0x30C8 + self->w->pos * 2);
    } else if (fl & 2) {
        if (self->w->mode != 1) {
            id = ((unsigned short *)((char *)self->w + 0x3064 + self->w->page * 20))[idx];
        } else {
            flag = 1;
        }
    } else {
        id = ((unsigned short *)((char *)self->w + 0x3064 + self->w->page * 20))[idx];
    }
    func_08ADBFB8(self, idx, id);
    func_08AF4E98(D_08B01CF8, flag);
    func_08ADB9A0(self);
}
}
