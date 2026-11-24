// Sets the flag at +0x2C and switches the sub-object's animation (0x10 then queue 0x11, else 0x11).
struct A { char pad[0xC]; short cur; short next; };
struct S { char pad[0x2C]; char flag; char pad2[7]; struct A *a; };
extern void func_088D9F5C(struct A *, int, int);
void func_08931204(struct S *self) {
    self->flag = 1;
    if (self->a != 0) {
        if (self->a->cur == 0x13) {
            func_088D9F5C(self->a, 0x10, 1);
            self->a->next = 0x11;
        } else {
            func_088D9F5C(self->a, 0x11, 1);
        }
    }
}
