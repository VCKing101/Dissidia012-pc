// Locks the global's mutex, reads the byte at +0x30 of the indexed 0x3C-byte slot, unlocks and returns it.
struct Slot { char pad[0x30]; unsigned char b; char pad2[0xB]; };
struct G { char pad[0x2C]; int mutex; char pad2[0x148]; struct Slot s[32]; };
extern struct G *D_089DA420;
extern void func_089C5FD8(int, int, int);
extern void func_089C5FC0(int, int);
unsigned char func_08990DE0(int i) {
    unsigned char r;
    func_089C5FD8(D_089DA420->mutex, 1, 0);
    r = D_089DA420->s[i].b;
    func_089C5FC0(D_089DA420->mutex, 1);
    return r;
}
