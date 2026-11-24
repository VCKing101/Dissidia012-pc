// Initialises the 0x154-byte slot for an index, marks it used in the shared block and sets flag 0x200 in the global state word.
struct Slot { char data[0x154]; };
struct Pool { char pad[0x3E0B0]; Slot slots[1]; };
struct Shared { char pad[0x24978]; unsigned char used[1]; };
struct Owner { char pad[0x138]; Shared *shared; };
struct State { int w0; int w4; unsigned flags; };
extern Pool *D_089D712C;
extern State *D_089D726C;
extern "C" {
extern void func_0892A198(Slot *, int);
extern void func_0892A17C(Slot *);
void func_08B0FF54(Owner *self, int idx, int arg) {
    Slot *s = (Slot *)((char *)&D_089D712C->slots[0] + idx * 0x154);
    func_0892A198(s, arg);
    func_0892A17C(s);
    self->shared->used[idx] = 1;
    unsigned *fl = &D_089D726C->flags;
    *fl |= 0x200;
}
}
