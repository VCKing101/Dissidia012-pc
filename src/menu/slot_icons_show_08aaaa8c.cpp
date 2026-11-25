/* Menu slot icons: for each of five slots hides both icon handles, then shows the ones whose count is positive and whose bit is set in the owner masks. */
struct Cnt { int v; int x; };
struct W {
    char p0[0xA0]; int a[5]; int b[5];
    char p1[0x788 - 0xC8]; Cnt ca[5];
    char p2[0x8B4 - 0x7B0]; Cnt cb[5];
};
struct Own { char p0[0x3C]; unsigned m3C; char p1[0x74]; unsigned mB4; };
struct Self { char p[0x7CFC]; W *w; };
extern "C" {
Own *func_08AABC54(void);
void func_088D9CFC(int);
void func_088D9CD8(int);
void func_08AAAA8C(Self *self)
{
    Own *o = func_08AABC54();
    int i = 0;
    do {
        func_088D9CFC(self->w->a[i]);
        if (self->w->ca[i].v > 0) {
            if ((o->m3C & (1 << i)) != 0) {
                func_088D9CD8(self->w->a[i]);
            }
        }
        func_088D9CFC(self->w->b[i]);
        if (self->w->cb[i].v > 0) {
            if ((o->mB4 & (1 << i)) != 0) {
                func_088D9CD8(self->w->b[i]);
            }
        }
        i++;
    } while (i < 5);
}
}
