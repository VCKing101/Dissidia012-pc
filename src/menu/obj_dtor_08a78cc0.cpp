/* Deleting destructor: sets the vtable, destroys the embedded member arrays, runs the base destructor and frees on request. */
extern "C" {
extern unsigned char D_08A796E0[];
extern void func_088D31D0(void *, int);
extern void func_088D2E7C(void *, int);
extern void func_08992F04(void *);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_08A775D8(void *, int);
}

struct ObjCC0 {
    void *vtbl;
    char sub[0x10];

    static void destroy(ObjCC0 *object, int flags)
    {
        if (object) {
            object->vtbl = D_08A796E0;
            char *m154 = (char *)object + 0x154;
            if (m154) {
                char *m16C = (char *)object + 0x16C;
                if (m16C) {
                    func_088D2E7C(m16C, 2);
                }
                func_0899372C((char *)object + 0x168, 1, 4, (void *)func_088D31D0, 0, 0);
                func_0899372C(m154, 5, 4, (void *)func_088D2E7C, 0, 0);
            }
            if ((char *)object + 4) {
                char *m124 = (char *)object + 0x124;
                if (m124) {
                    func_0899372C((char *)object + 0x138, 4, 4, (void *)func_088D31D0, 0, 0);
                    func_0899372C(m124, 5, 4, (void *)func_088D2E7C, 0, 0);
                }
                if ((char *)object + 0x120) {
                    func_088D2E7C((char *)object + 0x120, 2);
                }
                func_0899372C((char *)object + 0x30, 10, 0x18, (void *)0x08A78518, 0, 0);
                func_0899372C((char *)object + 0x18, 6, 4, (void *)func_088D31D0, 0, 0);
                func_0899372C((char *)object + 4, 5, 4, (void *)func_088D2E7C, 0, 0);
            }
            func_08A775D8(object, 0);
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_08A78CC0(ObjCC0 *object, int flags)
{
    ObjCC0::destroy(object, flags);
}
