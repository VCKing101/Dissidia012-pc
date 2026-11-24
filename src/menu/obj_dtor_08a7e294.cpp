/* Deleting destructor: sets the vtable, destroys the nested members in reverse order, runs the base destructor and frees on request. */
extern "C" {
extern unsigned char D_08A7E370[];
extern void func_088D31D0(void *, int);
extern void func_088D2E7C(void *, int);
extern void func_08A7DD58(void);
extern void func_08992F04(void *);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_08A775D8(void *, int);
}

struct ObjE294 {
    void *vtbl;
    char sub[0x10];

    static void destroy(ObjE294 *object, int flags)
    {
        if (object) {
            object->vtbl = D_08A7E370;
            char *m4 = (char *)object + 4;
            if (m4) {
                char *m14 = (char *)object + 0x14;
                char *m7C = (char *)object + 0x7C;
                if (m7C) {
                    func_088D2E7C(m7C, 2);
                }
                if (m14) {
                    func_0899372C(m14, 3, 0x20, (void *)func_08A7DD58, 0, 0);
                }
                func_088D31D0((char *)object + 0x10, 2);
                func_0899372C(m4, 3, 4, (void *)func_088D2E7C, 0, 0);
            }
            func_08A775D8(object, 0);
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_08A7E294(ObjE294 *object, int flags)
{
    ObjE294::destroy(object, flags);
}
