/* Deleting destructor with three embedded member arrays. */
extern "C" {
extern unsigned char D_08A7E9C0[];
extern void func_088D31D0(void *, int);
extern void func_088D2E7C(void *, int);
extern void func_08992F04(void *);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_08A775D8(void *, int);
}

struct ObjE82C {
    void *vtbl;
    char sub[0x60];

    static void destroy(ObjE82C *object, int flags)
    {
        if (object) {
            object->vtbl = D_08A7E9C0;
            char *m30 = (char *)object + 0x30;
            char *m14 = (char *)object + 0x14;
            char *m4 = (char *)object + 4;
            if (m30) {
                if (m30) {
                    if ((char *)object + 0x48) func_088D2E7C((char *)object + 0x48, 2);
                    func_0899372C((char *)object + 0x44, 1, 4, (void *)func_088D31D0, 0, 0);
                    func_0899372C(m30, 5, 4, (void *)func_088D2E7C, 0, 0);
                }
            }
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

extern "C" void func_08A7E82C(ObjE82C *object, int flags)
{
    ObjE82C::destroy(object, flags);
}
