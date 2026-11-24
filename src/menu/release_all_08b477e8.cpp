// Looks up the entry count for a source and releases that many handles from an array, returning the count.
struct Obj { char pad[0x18]; int field18; };
extern "C" {
extern int func_08B4A90C(int, int, int *, int);
extern void func_0892538C(int, int);
int func_08B477E8(Obj *self, int key, int *handles, int arg) {
    if (!key) {
        return 0;
    }
    int n = func_08B4A90C(self->field18 + 0x20, key, handles, arg);
    for (int i = 0; i < n; i++, handles++) {
        func_0892538C(*handles, 0);
    }
    return n;
}
}
