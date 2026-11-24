/* Base init, stores two arguments, then fills five slots with lookups of a key and finishes with a second init. */
struct Obj { int pad0; int slot[5]; int a18; int a1c; };
extern void func_08B50400(struct Obj *);
extern int func_088DAA68(int);
extern void func_088DAB7C(int, int);
extern void func_08B50454(struct Obj *);
void func_08B4FECC(struct Obj *self, int key, int a, int b) {
    int i;
    func_08B50400(self);
    self->a18 = a;
    self->a1c = b;
    for (i = 0; i < 5; i++) {
        self->slot[i] = func_088DAA68(key);
        func_088DAB7C(key, self->slot[i]);
    }
    func_08B50454(self);
}
