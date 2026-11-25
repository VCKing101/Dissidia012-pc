/* Menu page teardown: clears six state words, releases the 0x2E handles and 0x39 objects of the block, then frees the block. */
struct W { char p0[4]; int objs[0x39]; char p1[0xEC - 0xE8]; int hs[0x2E]; char p2[0x2FB4 - 0x1A4]; int st[6]; };
struct Self { W *w; void *f4; };
extern "C" {
void func_08B26F5C(void);
void func_08B25390(Self *);
void func_0887BA5C(int);
void func_08B259B8(Self *, int, int, int);
void func_088DA060(int);
void func_0885C538(void *, void *, int);
void func_08B2513C(Self *self)
{
    int i;
    func_08B26F5C();
    func_08B25390(self);
    for (i = 0; i < 6; i++) {
        self->w->st[i] = 0;
    }
    for (i = 0; i < 0x2E; i++) {
        if (self->w->hs[i] != 0) {
            func_0887BA5C(self->w->hs[i]);
            self->w->hs[i] = 0;
        }
    }
    func_08B259B8(self, 1, -1, 0);
    for (i = 0; i < 0x39; i++) {
        if (self->w->objs[i] != 0) {
            func_088DA060(self->w->objs[i]);
            self->w->objs[i] = 0;
        }
    }
    func_0885C538(self->f4, self->w, 0);
    self->w = 0;
}
}
