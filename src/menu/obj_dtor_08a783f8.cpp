/* Deleting destructor: sets the vtable, destroys the embedded member arrays, runs the base destructor and frees on request. */
extern "C" {
extern unsigned char D_08A784E8[];
extern void func_088D31D0(void *, int);
extern void func_088D2E7C(void);
extern void func_08992F04(void *);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_08A775D8(void *, int);
}

struct ObjF3F8 {
    void *vtbl;
    char subA[0x10];
    char subB[0x20];
    char arrC[0x20];

    static void destroy(ObjF3F8 *object, int flags)
    {
        if (object) {
            object->vtbl = D_08A784E8;
            char *m14 = (char *)object + 0x14;
            char *m4 = (char *)object + 4;
            if (m14) {
                func_0899372C((char *)object + 0x2C, 1, 4, (void *)func_088D31D0, 0, 0);
                func_0899372C(m14, 6, 4, (void *)func_088D2E7C, 0, 0);
            }
            if (m4) {
                func_088D31D0((char *)object + 0x10, 2);
                func_0899372C(m4, 3, 4, (void *)func_088D2E7C, 0, 0);
            }
            func_08A775D8(object, 0);
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_08A783F8(ObjF3F8 *object, int flags)
{
    ObjF3F8::destroy(object, flags);
}
