/* Stores a value and clears a flag on the object, runs func_08B3E840, then forwards the float to func_0880FD34. */
extern void func_08B3E840(void *obj, int z);
extern void func_0880FD34(float f, void *p, int a, int b);
typedef struct { char pad[0x50]; int v; unsigned char flag; } Obj;
void func_08B3EFCC(float f, Obj *obj, char *other, int v, int w, int z) {
    obj->v = v;
    obj->flag = 0;
    func_08B3E840(obj, z);
    func_0880FD34(f, other + 0x1E0, v, w);
}
