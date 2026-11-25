/* Re-positions an attached effect object from its owner's slot data. */
typedef struct Row { char pad[8]; int v; char pad2[0x1C - 0xC]; } Row;
typedef struct Tab { char pad0[0xC]; Row *rows; char pad1[0x2C - 0x10]; int count; } Tab;
typedef struct Def { char pad0[0xC]; short idx; char pad1[0x24 - 0x0E]; struct Def2 *d2; } Def;
typedef struct Def2 { char pad0[0xC]; Tab *tab; } Def2;
typedef struct Obj { Def *def; int a; int fx; } Obj;
extern "C" void func_088DA0A8(Def *, int, void *, void *);
extern "C" void func_0887B778(int, void *);
extern "C" void func_0887B828(int, void *);
extern "C" void func_0887B970(int);
extern "C" void func_0887B9A4(int);
extern "C" void func_0887B900(int, int);
struct H { static void run(Obj *self)
{
    char buf[0x40];
    int loc;
    if (self->fx != 0) {
        Tab *t;
        int idx;
        int v;
        func_088DA0A8(self->def, 0, buf, &loc);
        func_0887B778(self->fx, buf);
        func_0887B828(self->fx, &loc);
        func_0887B970(self->fx);
        func_0887B9A4(self->fx);
        {
            Def *d = self->def;
            int fx = self->fx;
            idx = d->idx;
            t = d->d2->tab;
            if (idx < t->count) v = t->rows[idx].v; else { Row *z; z = 0; v = z->v; }
            func_0887B900(fx, v);
        }
    }
}
};
extern "C" void func_088CC578(Obj *self) { H::run(self); }
