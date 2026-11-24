/* Clears every slot of the two handle arrays (calling the release callback on live ones), frees the block and finishes. */
struct Pool {
    int *arr0; int h1; int p2; int *arr3; int p4; int count; int p6; int p7; void (*release)(int);
};
extern "C" void func_089C26E4(void);
extern "C" void func_089C26A4(void);
extern "C" void func_08993754(int h, int a, int b, void (*c)(void), void (*d)(void), int e);
extern "C" void func_08942544(Pool *p);
extern "C" void func_08942A50(Pool *self)
{
    for (int i = 0, off = 0; i < self->count; i++, off += 4) {
        int *a = (int *)((char *)self->arr0 + off);
        if (*a != 0) {
            *a = 0;
        }
        int *b = (int *)((char *)self->arr3 + off);
        if (*b != 0) {
            self->release(*b);
            *(int *)((char *)self->arr3 + off) = 0;
        }
    }
    if (self->h1 != 0) {
        func_08993754(self->h1, -1, 0x10, func_089C26E4, func_089C26A4, 0);
        self->h1 = 0;
    }
    func_08942544(self);
}
