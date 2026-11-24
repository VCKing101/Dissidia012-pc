// Per-frame poll: when the helper is ready, the debug byte or a second check passes and the queue accepts, sets state 3 on the sub-object. Returns 0.
struct Sub { char pad[0x594]; int state; };
struct Owner { char pad[0x850]; Sub *sub; };
extern int D_08B01CF8;
extern unsigned char D_089D5F20;
extern int D_089D69E8;
extern "C" {
extern void func_08AF7084(int);
extern int func_08AF70C4(int);
extern int func_08AF7504(int);
extern int func_08885608(int, int);
int func_08ADFCE0(Owner *self) {
    func_08AF7084(D_08B01CF8);
    if (!func_08AF70C4(D_08B01CF8)) {
        return 0;
    }
    if (!(D_089D5F20 != 0 || func_08AF7504(D_08B01CF8))) {
        return 0;
    }
    if (!func_08885608(D_089D69E8, 1)) {
        return 0;
    }
    self->sub->state = 3;
    return 0;
}
}
