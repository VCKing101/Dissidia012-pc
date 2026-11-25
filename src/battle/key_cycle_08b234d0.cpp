// Tests two animation keys; when the first is clear and the second set, steps a 0..9 counter, then runs the update helpers.
struct Anim08B234D0 {
    char data[0x34];
    virtual int slot0(unsigned long long k);
    virtual int slot1(unsigned long long k);
};
struct Self08B234D0 {
    Anim08B234D0 *anim;
    char pad4[0xC];
    char box[0x20];
    unsigned char cnt;
};
extern "C" {
extern unsigned long long D_08B27868;
extern unsigned long long D_08B27870;
extern void func_08B249B8(void *self);
extern void func_08B253FC(void *self, void *a, void *b, void *c, void *d);
extern void func_08B23E3C(void *self, int a, int b);
extern void func_08B232FC(void *self, int a);
void func_08B234D0(Self08B234D0 *self)
{
    if (self->anim->slot0(D_08B27868) == 0) {
        if (self->anim->slot1(D_08B27870) != 0) {
            self->cnt = self->cnt + 1;
            if (self->cnt >= 10) {
                self->cnt = 0;
            }
        }
    }
    func_08B249B8(self);
    func_08B253FC(self, self->box, self->box, self->box, self->box);
    func_08B23E3C(self, 1, self->cnt);
    func_08B232FC(self, 0);
}
}
