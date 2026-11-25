/* Loads "win_lose_condition_param.bin": func_08862C40(name, 1), creates the request with func_0885C6CC(field 4, file, 0, 0, 1, 0) into field 0x14, then func_08862854(name, request, file, 1). */
typedef struct Obj089275cc { unsigned char pad0[4]; void *owner; unsigned char pad1[0x14 - 8]; void *request; } Obj089275cc;
extern char D_089CEAC0[];
extern void *func_08862C40(char *name, int a);
extern void *func_0885C6CC(void *owner, void *file, int a, int b, int c, int d);
extern void func_08862854(char *name, void *req, void *file, int a);
void func_089275CC(Obj089275cc *self)
{

    void *file = func_08862C40(D_089CEAC0, 1);
    int one = 1;
    self->request = func_0885C6CC(self->owner, file, 0, 0, one, 0);
    func_08862854(D_089CEAC0, self->request, file, 1);
}
