/* Per-mode update step: mode 0 refreshes a flag, 1 and 3 refresh then notify, 2 refreshes and queues a message. */
typedef struct Ms { char pad0[0xFC]; int mode; char pad1[0x128 - 0x100]; int f128; char pad2[0x154 - 0x12C]; int f154; int f158; int f15C; int f160; char pad3[0x168 - 0x164]; int f168; char pad4[0x170 - 0x16C]; unsigned char flag; } Ms;
extern int D_08A4FFF4;
extern void func_08A4D1C8(int, int, int);
extern void func_08A4D1F8(int, int);
extern void func_08A4E7CC(int, int, int);
extern void func_08A4E8B8(int, int, int, int);
extern void func_08A5E13C(Ms *);
extern void func_08A5E640(Ms *);
extern void func_08A5EC78(Ms *);
extern void func_08A5ED84(Ms *);
extern void func_08A5EE5C(Ms *);
void func_08A5E6B8(Ms *self) {
    switch (self->mode) {
    case 0:
        self->flag = self->f128 == 0;
        func_08A5EC78(self);
        func_08A5ED84(self);
        break;
    case 1:
        func_08A5EE5C(self);
        func_08A5E13C(self);
        func_08A4E8B8(D_08A4FFF4, self->f168, self->f158, self->f15C);
        break;
    case 2:
        func_08A5E13C(self);
        func_08A4D1F8(D_08A4FFF4, self->f160);
        func_08A4D1C8(D_08A4FFF4, 4, -1);
        break;
    case 3:
        func_08A5EE5C(self);
        func_08A4E7CC(D_08A4FFF4, self->f154, self->f160);
        func_08A5E640(self);
        break;
    }
}
