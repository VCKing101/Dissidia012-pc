/* Deleting destructor: sets the vtable, destroys the embedded members, runs the base destructor and frees on request. */
extern "C" {
extern unsigned char D_08A7AA20[];
extern void func_088D31D0(void *, int);
extern void func_088D2E7C(void);
extern void func_08992F04(void *);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_08A775D8(void *, int);
}

struct ObjA984 {
    void *vtbl;
    char sub[0x0C];
    char sub2[1];

    static void destroy(ObjA984 *object, int flags)
    {
        if (object) {
            object->vtbl = D_08A7AA20;
            if (object->sub) {
                func_088D31D0((char *)object + 0x10, 2);
                func_0899372C(object->sub, 3, 4, (void *)func_088D2E7C, 0, 0);
            }
            func_08A775D8(object, 0);
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_08A7A984(ObjA984 *object, int flags)
{
    ObjA984::destroy(object, flags);
}
