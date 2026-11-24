/* When func_088AC9A8 reports 1, calls func_08B081D4 with mode 0xE or 0x10 depending on the object's +0x10 field. */
typedef struct Obj { char pad[0x10]; int field; } Obj;
extern void *D_089D712C;
extern int func_088AC9A8(void *p);
extern void func_08B081D4(void *self, int mode);

int func_08B07ADC(Obj *self)
{
    if (func_088AC9A8(D_089D712C) == 1) {
        if (self->field != 0) {
            func_08B081D4(self, 0xE);
        } else {
            func_08B081D4(self, 0x10);
        }
    }
    return 0;
}
