// Frees the pointer at +0 with func_088DA060, then releases each of five pointers at +4..+0x14 with func_0887BA5C, clearing them.
extern void func_088DA060(void *);
extern void func_0887BA5C(void *);
void func_08B121DC(void **self)
{
    int i;
    if (*self != 0) {
        func_088DA060(*self);
        *self = 0;
    }
    for (i = 0; i < 5; i++) {
        if (self[1] != 0) {
            func_0887BA5C(self[1]);
            self[1] = 0;
        }
        self++;
    }
}
