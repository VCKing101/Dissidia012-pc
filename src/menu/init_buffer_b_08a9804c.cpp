// Unless the mode is 3 and the second word is 2, allocates a 0x270800-byte block and registers it as "CROOM_T_BUF".
extern "C" {
extern struct Blk { int mode; char g[0x1C8]; int second; } D_09B6EFB0;
int func_0888204C(int size);
int func_0885C40C(int block, int size, const char *name, int n);
extern const char D_08AA3CEC[];
}
template <class T> struct Kept { T value; Kept(T v) : value(v) {} };
extern "C" void func_08A9804C(int self) {
    if (D_09B6EFB0.mode != 3 || D_09B6EFB0.second != 2) {
        Kept<int> saved(self);
        int blk = func_0888204C(0x270800);
        *(int *)(saved.value + 0x134) = blk;
        int r = func_0885C40C(blk, 0x270800, D_08AA3CEC, 0x40);
        *(int *)(saved.value + 0x138) = r;
    }
}
