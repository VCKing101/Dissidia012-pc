// Starts the next of six 0x5C-byte slots with its table entry; returns 0 when the index is out of range.
struct S { char pad[0x30]; int a; int b; char slots[0x5C * 6]; int n; };
extern void func_089303E4(void *, int, int, int, int);
extern unsigned short D_089CF1DC[];
int func_08930D78(struct S *self, int x) {
    int n = self->n;
    if (n < 0 || 6 < n) {
        return 0;
    }
    func_089303E4(self->slots + n * 0x5C, self->a, self->b, D_089CF1DC[n], x);
    self->n = self->n + 1;
    return 1;
}
