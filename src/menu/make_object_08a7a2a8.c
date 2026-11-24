/* Allocates a 0xB00-byte object from the heap, stores its vtable and runs the member initialisers; returns it or null. */
extern void func_08993560(void *p, int a, int b, void *fn);
extern void func_088D2E70(void);
extern void func_088D31C4(void);
extern void func_08A7A0D0(void);
extern void func_08A734B0(void *p);
extern void func_08A73C38(void *p);
extern int func_0885C6CC(int heap, int size, int a, int b, int c, int d);
extern char D_08A796E0[];
int *func_08A7A2A8(int heap) {
    int one = 1;
    int *none = 0;
    int *result[1];
    int *made;
    int *self;
    result[0] = none;
    self = (int *)func_0885C6CC(heap, 0xB00, 0, 0, one, 0);
    made = result[0];
    if (self != 0) {
        *self = (int)D_08A796E0;
        func_08A734B0(self + 1);
        func_08993560(self + 0x55, 5, 4, (void *)func_088D2E70);
        func_08993560(self + 0x5A, 1, 4, (void *)func_088D31C4);
        func_08A73C38(self + 0x5B);
        func_08993560(self + 0x5F, 10, 0xF0, (void *)func_08A7A0D0);
        made = self;
    }
    return made;
}
