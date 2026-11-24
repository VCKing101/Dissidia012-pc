// Locks the global's mutex, starts the command on the indexed slot when its state is 0x10, unlocks; returns 0.
struct Slot { int state; char pad[0x38]; };
struct G { char pad[0x2C]; int mutex; char pad2[0x148]; struct Slot s[32]; };
extern struct G *D_089DA420;
extern void func_089C5FD8(int, int, int);
extern void func_089C5FC0(int, int);
extern void func_089A504C(int, unsigned char);
int func_089908D8(int i, unsigned char c) {
    func_089C5FD8(D_089DA420->mutex, 1, 0);
    if (D_089DA420->s[i].state == 0x10) {
        func_089A504C(i, c);
    }
    func_089C5FC0(D_089DA420->mutex, 1);
    return 0;
}
