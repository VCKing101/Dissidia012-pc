/* When active, advances the page counter and plays a direction-dependent sound if it changed. */
typedef struct Pt { unsigned char on0; char pad1; unsigned char on2; char pad3[0x24 - 3]; int s24; int s28; char pad2[0x74 - 0x2C]; char c74[4]; int page; } Pt;
extern void func_088D688C(void *, int);
extern void func_088D9F5C(int, int, int);
extern void func_08A63C74(Pt *);
extern void func_08A64154(Pt *);
void func_08A63D84(Pt *self) {
    int old;
    if (self->on2 == 0) return;
    if (self->on0 == 0) return;
    old = self->page;
    func_088D688C(self->c74, 0);
    if (old != self->page) {
        func_08A64154(self);
        func_08A63C74(self);
        if (old + 1 == self->page) {
            func_088D9F5C(self->s28, 0x1A, 1);
        } else if (old - 1 == self->page) {
            func_088D9F5C(self->s24, 0x18, 1);
        } else if (self->page < old) {
            func_088D9F5C(self->s28, 0x1A, 1);
        } else if (old < self->page) {
            func_088D9F5C(self->s24, 0x18, 1);
        }
    }
}
