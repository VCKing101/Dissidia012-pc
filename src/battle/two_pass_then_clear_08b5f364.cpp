// Runs func_088984E8 on the func_08899300 object with field 0x14 of each of two 0x1C-byte entries, then func_08B5FD78 on +0x44.
struct Ent { char pad[0x14]; int v; char pad2[4]; };
struct Obj { Ent e[2]; };
extern "C" void *func_08899300(void);
extern "C" void func_088984E8(void *, int);
extern "C" void func_08B5FD78(void *);
extern "C" void func_08B5F364(Obj *self)
{
    for (int i = 0; i < 2; i++) {
        func_088984E8(func_08899300(), self->e[i].v);
    }
    func_08B5FD78((char *)self + 0x44);
}
