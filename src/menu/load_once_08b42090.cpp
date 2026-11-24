// On first call loads each entry of a one-element table into the object's slots and marks it loaded.
struct Obj { int slot[1]; unsigned char loaded; };
extern int D_089D6D78;
extern int D_08B4228C[];
extern "C" {
extern int func_088A5378(int, int);
extern void func_088A9A80(int);
void func_08B42090(Obj *self) {
    if (self->loaded) {
        return;
    }
    for (int i = 0; i < 1; i++) {
        self->slot[i] = func_088A5378(D_089D6D78, D_08B4228C[i]);
        func_088A9A80(self->slot[i]);
    }
    self->loaded = 1;
}
}
