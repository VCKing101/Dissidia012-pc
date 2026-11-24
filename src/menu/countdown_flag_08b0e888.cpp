// Per-frame countdown: when the counter in the shared block goes negative and the helper reports ready, sets a flag. Returns 0.
struct Shared { char pad[0x1F898]; int flag; char pad2[8]; int count; };
struct Owner { char pad[0x138]; Shared *shared; };
extern int D_08B01CF8;
extern "C" {
extern void func_08AF7084(int);
extern int func_08AF70C4(int);
int func_08B0E888(Owner *self) {
    func_08AF7084(D_08B01CF8);
    self->shared->count = self->shared->count - 1;
    if (self->shared->count >= 0) {
        return 0;
    }
    if (!func_08AF70C4(D_08B01CF8)) {
        return 0;
    }
    self->shared->flag = 1;
    return 0;
}
}
