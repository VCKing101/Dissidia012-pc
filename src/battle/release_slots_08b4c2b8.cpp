// Releases the two pointers at +4 and +8 with func_0887BA5C, then the pointer at +0 with func_088DA060, clearing each.
extern "C" void func_0887BA5C(void *);
extern "C" void func_088DA060(void *);
typedef struct { void *main; void *slot[2]; } Obj08B4C2B8;
extern "C" void func_08B4C2B8(Obj08B4C2B8 *self)
{

    for (int i = 0; i < 2; i++) {
        if (self->slot[i] != 0) {
            func_0887BA5C(self->slot[i]);
            self->slot[i] = 0;
        }
    }
    if (self->main != 0) {
        func_088DA060(self->main);
        self->main = 0;
    }
}
