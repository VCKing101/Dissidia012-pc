struct Obj088D78F4 { char pad0[0x10]; int state; char pad1[0x10]; char sub[0x38]; char list[4]; };
typedef void (*Cb088D78F4)(void);
extern "C" int func_088D82B8(void *);
extern "C" unsigned func_088D82F0(void *);
extern "C" unsigned func_088D93E0(void *);
extern "C" void *func_088D9418(void *, unsigned);
extern "C" Cb088D78F4 func_088D8EC8(void *);
extern "C" int func_088D8F00(void *);
extern "C" void func_088D84BC(void *);
extern "C" void func_088D7B00(Obj088D78F4 *);
extern "C" int func_088D78F4(Obj088D78F4 *self);
extern "C" int func_088D78F4(Obj088D78F4 *self)
{
    char *q = self->sub;
    int st = func_088D82B8(q);
    if (st != 0) {
        int ok = 0;
        if (st == 1) {
            unsigned idx = func_088D82F0(q);
            char *lst = self->list;
            if (idx < func_088D93E0(lst)) {
                void *e = func_088D9418(lst, idx);
                if (e != 0) {
                    Cb088D78F4 cb = func_088D8EC8(e);
                    if (cb != 0) {
                        if (func_088D8F00(e) != 0) {
                            cb();
                            ok = 1;
                        }
                    }
                }
            }
        }
        if (!ok) {
            func_088D7B00(self);
        } else {
            func_088D84BC(q);
            self->state = 6;
        }
    }
    return 0;
}
