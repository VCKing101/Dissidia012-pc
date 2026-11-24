/* Starts the object with the given index: picks its source (a helper in mode 9, else the +0x5A0 pointer), runs func_08B1EBE8, sets state 1. */
typedef struct StartSrc { char pad[0x5A0]; void *src; } StartSrc;
typedef struct StartSelf { char pad[4]; int state; char pad_08[4]; StartSrc *owner; unsigned char flag; char pad_11[0x27]; int h38; } StartSelf;
typedef struct Mode { char pad[0x4C]; int mode; } Mode;
extern Mode *D_089D6A10;
extern void *func_088720A4(void);
extern void func_08B1EBE8(StartSelf *self, void *src, int index);
extern void func_088CCD50(int h);
int func_08B1DE34(StartSelf *self, int index)
{
    void *src;
    int one;
    if (index < 0) {
        return -1;
    }
    if (self->owner == 0) {
        return -2;
    }
    if (D_089D6A10->mode == 9) {
        src = func_088720A4();
    } else {
        src = self->owner->src;
        if (src == 0) {
            return -3;
        }
    }
    func_08B1EBE8(self, src, index);
    one = 1;
    if (self->flag == one) {
        func_088CCD50(self->h38);
    }
    self->state = one;
    return 0;
}
