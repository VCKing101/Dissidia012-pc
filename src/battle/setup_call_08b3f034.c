/* Copies a value into the object, sets a flag, then forwards to func_08B3EFCC. */
extern void func_089A3EFC(void *dst, int src);
extern void func_08B3EFCC(float f, void *obj, void *other, int v, int w, int z);
typedef struct { char pad[0x10]; char sub[0x44]; unsigned char flag; } Obj;
void func_08B3F034(float f, Obj *obj, void *other, int v, int src, int z) {
    func_089A3EFC(obj->sub, src);
    obj->flag = 1;
    func_08B3EFCC(f, obj, other, v, 0, z);
}
