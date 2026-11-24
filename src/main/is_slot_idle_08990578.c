// Locks the global's mutex, reports whether the slot at +0x8FC exists with its flag at +0x40 clear, and unlocks.
struct G { char pad[0x8FC]; struct { char pad[0x40]; unsigned char f; } *slot; int mutex; };
extern struct G *D_089DA420;
extern void func_089C5FD8(int, int, int);
extern void func_089C5FC0(int, int);
int func_08990578() {
    int r;
    func_089C5FD8(D_089DA420->mutex, 1, 0);
    r = 0;
    if (D_089DA420->slot != 0 && D_089DA420->slot->f == 0) {
        r = 1;
    }
    func_089C5FC0(D_089DA420->mutex, 1);
    return r;
}
