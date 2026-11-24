// Plays the sub-object's animation chosen by the flag at +0x2C, then steps each active slot.
struct S { char pad[0x2C]; unsigned char flag; char pad2[7]; void *a; char pad3[0x228]; int n; };
extern void func_088D9F5C(void *, int, int);
extern void func_0893071C(void *);
void func_08931328(struct S *self) {
    void *a;
    int i;
    char *p;
    a = self->a;
    if (a != 0) {
        if (self->flag == 0) {
            func_088D9F5C(a, 0x14, 1);
        } else {
            func_088D9F5C(a, 0x11, 1);
        }
    }
    for (i = 0, p = (char *)self + 0x38; i < self->n; i++, p += 0x5C) {
        func_0893071C(p);
    }
}
