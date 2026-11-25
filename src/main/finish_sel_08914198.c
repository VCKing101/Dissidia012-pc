/* Resets the fade (0 to 1), and when the pad object at +0x570 reports ready twice, finishes the selection: ends fade, runs step 2, sets +0x7D7 and clears bit 1 of the flags at +0x6D0->0x380. */
typedef struct Pl { char p[0x380]; unsigned int flags; } Pl;
typedef struct S { char p[0x570]; int pad; char q[0x15C]; Pl *pl; char r[0x103]; unsigned char done; } S;
extern void func_08864D98(float a, float b);
extern void func_08B49CA4(int pad);
extern int func_08B49E54(int pad);
extern int func_08B49EE4(int pad);
extern void func_08864DB8(void);
extern void func_08914B64(S *self, int n);
void func_08914198(S *self) {
    func_08864D98(0.0f, 1.0f);
    func_08B49CA4(self->pad);
    if (func_08B49E54(self->pad) != 0) {
        if (func_08B49EE4(self->pad) != 0) {
            func_08864DB8();
            func_08914B64(self, 2);
            self->done = 1;
            self->pl->flags = self->pl->flags & ~2;
        }
    }
}
