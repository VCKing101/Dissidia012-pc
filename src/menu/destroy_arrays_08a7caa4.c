/* If self is non-null, runs func_0899372C over its three embedded arrays (+0x10, +8, +0) with their destructors, then frees self when bit 0 of a1 is set. */
extern void func_0899372C(void *p, int n, int size, void *dtor, int a, int b);
extern void func_08992F04(void *p);
extern void func_08A7CA48(void);
extern void func_088D31D0(void);
extern void func_088D2E7C(void);
void func_08A7CAA4(char *self, unsigned int flags) {
    char *me = self;
    int fl = flags;
    if (self != 0) {
        func_0899372C(me + 0x10, 5, 0x14, func_08A7CA48, 0, 0);
        func_0899372C(me + 8, 2, 4, func_088D31D0, 0, 0);
        func_0899372C(me, 2, 4, func_088D2E7C, 0, 0);
        if ((fl & 1) != 0) {
            func_08992F04(me);
        }
    }
}
