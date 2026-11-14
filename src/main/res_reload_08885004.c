/* Replaces the object's resource with a newly loaded one and refreshes it. */
typedef struct Obj08885004 {
    char pad_00[0xC];
    void *res;
} Obj08885004;

void func_0887BA5C(void *res);
void *func_0887B3EC(int a, int b, int zero, int c, int zero2, int zero3);
void func_0887B9C4(void *res);
void func_0887B98C(void *res);
void func_08884F98(Obj08885004 *o);

void func_08885004(Obj08885004 *o, int a, int b, int c)
{
    if (o->res != 0) {
        func_0887BA5C(o->res);
        o->res = 0;
    }
    o->res = func_0887B3EC(a, b, 0, c, 0, 0);
    if (o->res != 0) {
        func_0887B9C4(o->res);
        func_0887B98C(o->res);
    }
    func_08884F98(o);
}
