typedef struct E11 { char pad[0x10]; unsigned flags; } E11;
typedef struct O11 { char pad0[0x30]; char a30[0x14]; int w44[1]; char pad1[0x1C]; int h64; E11 *e68; int h6c; E11 *e70; } O11;
extern char D_089CEE20[];
extern void func_0887BA5C(int h);
extern int func_0887B3EC(int a, int b, int c, int d, int e, int f);
extern E11 *func_0887E534(int h);
extern void func_088D428C(void *a, void *b, void *c, int d);

void func_0892D8F0(O11 *self, int a1, int a2, int sel) {
    int x = -1;
    int y = -1;
    int h;
    E11 *e;
    switch (sel) {
    case 0:
        break;
    case 1:
        x = 5;
        y = 6;
        break;
    case 2:
        x = 0x1B;
        break;
    case 3:
        x = 0x18;
        y = 6;
        break;
    }
    if (self->h64 != 0) {
        func_0887BA5C(self->h64);
        self->h64 = 0;
    }
    if ((int)self->e68 != 0) {
        func_0887BA5C((int)self->e68);
        self->e68 = 0;
    }
    if (self->h6c != 0) {
        func_0887BA5C(self->h6c);
        self->h6c = 0;
    }
    if ((int)self->e70 != 0) {
        func_0887BA5C((int)self->e70);
        self->e70 = 0;
    }
    if (x != -1) {
        h = func_0887B3EC(a1, a2, 0, x, 0, 0);
        self->h64 = h;
        e = func_0887E534(h);
        self->e68 = e;
        e->flags |= 0x800;
    }
    if (y != -1) {
        h = func_0887B3EC(a1, a2, 0, y, 0, 0);
        self->h6c = h;
        e = func_0887E534(h);
        self->e70 = e;
        e->flags |= 0x800;
    }
    func_088D428C(self->a30, self->w44, D_089CEE20, 1);
}
