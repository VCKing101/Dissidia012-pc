struct Pos { float x, y, z; char pad[0x24 - 0xC]; void *eff; };
struct Info { char pad[0xD]; signed char kind; };
struct Obj { unsigned flags; Pos *pos; };
struct Vec3 { float x, y, z; };
extern "C" void *func_08B43E28(int size);
extern "C" void *func_08B45808(void *a, int b);
extern "C" void func_08B43FC8(void *p, void *q);
extern "C" void func_08B443B8(void *eff, Vec3 *v);
extern "C" void *D_08B45878;
extern "C" const float D_08B4BFE0;

struct Mk { static void *make() {
        void *r = 0;
        void *p = func_08B43E28(0x1C);
        if (p != 0) {
            func_08B43FC8(p, func_08B45808(D_08B45878, 2));
            r = p;
        }
        return r;
    }
};

extern "C" void func_08925184(Obj *self) {
    int c = ((Info *)self->pos)->kind;
    switch (c) {
    case 1:
        break;
    case 2:
        break;
    default:
        self->flags = self->flags & 0xFFFFFFFE;
        break;
    }
    {
        void *r = Mk::make();
        Vec3 v;
        Pos *q;
        self->pos->eff = r;
        q = self->pos;
        v = *(Vec3 *)q;
        v.y = v.y + D_08B4BFE0;
        func_08B443B8(q->eff, &v);
    }
}
