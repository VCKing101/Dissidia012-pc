typedef struct KindPlayer {
    unsigned char active;
    char pad_01[0x3];
    void *target;
    int field_08;
} KindPlayer;

extern int D_089F2000[];
extern unsigned int func_08A22CA8(int id);
extern void func_088D9F5C(void *target, int value, int count);
extern void func_089F1D2C(KindPlayer *self);

int func_089F1F24(KindPlayer *self)
{
    unsigned int kind;
    if (!self->active) {
        return -1;
    }
    if (self->target == 0) {
        return -2;
    }
    kind = func_08A22CA8(self->field_08);
    if (kind >= 2) {
        return -3;
    }
    func_088D9F5C(self->target, D_089F2000[kind], 1);
    func_089F1D2C(self);
    return 0;
}
