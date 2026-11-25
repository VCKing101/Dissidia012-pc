struct Rq088C1F6C {
    int kind;
    int a;
    int b;
    short c;
    char pad[0x12C];
    short d;
};
class C088C1F6C {
public:
    char data[4];
    int fn(Rq088C1F6C *, int, int, int *);
};
typedef int (C088C1F6C::*H088C1F6C)(Rq088C1F6C *, int, int, int *);
struct Tbl088C1F6C { H088C1F6C h[10]; };
extern "C" {
void func_08935524(void *dst, const void *src, int size);
void func_088C2690(void *self, int arg, Rq088C1F6C *rq);
extern const Tbl088C1F6C D_089D7490;
extern int D_089D712C;

bool func_088C1F6C(C088C1F6C *self, Rq088C1F6C *rq, int p3, int *p4, int p5)
{
    Tbl088C1F6C table;
    int unused[1];
    func_08935524(&table, &D_089D7490, sizeof(table));
    rq->kind = -1;
    rq->b = 0;
    rq->a = -1;
    rq->c = 0;
    rq->d = 0;
    int k = *p4;
    rq->kind = k;

    int r = (self->*table.h[k])(rq, p3, p5, p4);
    if (r == 1) {
        func_088C2690(self, D_089D712C + 0xF58, rq);
        return true;
    }
    return false;
}
}
