// Resets the owner's state when its sub-object is idle, then re-enters the sub-object; always returns 0.
struct Self {
    char pad0[4]; int i04;
    char pad1[0xB4 - 8]; int ib4, ib8;
    char pad2[4]; int ic0, ic4; float fc8;
    char pad3[0xE4 - 0xCC]; char be4; char pad4[3]; float fe8, fec; char bf0;
    char pad5[0x148 - 0xF1]; char sub[0x158 - 0x148]; int h158, h15c, h160;
};
extern int func_08A7E904(void *sub, int a);
extern void func_08A7E858(void *sub, int a);
extern void func_08A9B928(struct Self *self);
extern void func_08A65A84(int h);
extern void func_08A66390(int h);
extern int func_08AB3274(int h);
extern int func_08A826F4(int h, int a);
extern int func_08AB3264(int h);
extern void func_08A97910(int h, int a, int b);

int func_08A9B4CC(struct Self *self)
{
    int zero = 0;
    char *sub = self->sub;
    if (func_08A7E904(sub, zero) == 0) {
        int none;
        func_08A9B928(self);
        none = -1;
        self->i04 = none;
        self->ib4 = 0;
        self->ib8 = 0;
        func_08A65A84(self->h158);
        func_08A66390(self->h15c);
        {
            float z = 0.0f;
            self->be4 = 0;
            self->fc8 = z;
            self->fe8 = z;
            self->fec = z;
        }
        self->bf0 = 0;
        if (func_08A826F4(func_08AB3274(self->h160), 0) == 1) {
            self->ic0 = none;
            self->ic4 = 0;
        }
        func_08A97910(func_08AB3264(self->h160), 0, -1);
    }
    func_08A7E858(sub, zero);
    return 0;
}
