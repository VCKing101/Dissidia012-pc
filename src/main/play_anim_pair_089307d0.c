// When the animation at +0x14 is 0x17, restarts it and starts the table entry (by index at +0x54) on the object at +0x10.
struct A { char pad[0xC]; short cur; };
struct S { char pad[0x10]; void *o; struct A *a; char pad2[0x3C]; int idx; };
extern void func_088D9F5C(void *, int, int);
extern unsigned short D_089CF1B8[][2];
void func_089307D0(struct S *self) {
    if (self->a->cur == 0x17) {
        func_088D9F5C(self->a, 0x17, 1);
        func_088D9F5C(self->o, D_089CF1B8[self->idx][1], 1);
    }
}
