/* Allocates the "OPT_MENU" buffer from the global allocator, or clears the member when there is none. */
extern struct Blk { int a, b; } D_09B6D818;
extern int func_0885C40C(int alloc, int size, const char *name, int n);
extern const char D_08ABB948[];
void func_08AB99DC(int self) {
    int keep[1];
    int alloc = D_09B6D818.b;
    if (alloc != 0) {
        int r;
        keep[0] = self;
        r = func_0885C40C(alloc, 0xBA800, D_08ABB948, 0x40);
        self = keep[0];
        *(int *)(self + 0x134) = r;
    } else {
        *(int *)(self + 0x134) = 0;
    }
}
