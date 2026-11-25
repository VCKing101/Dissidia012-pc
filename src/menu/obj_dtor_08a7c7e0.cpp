/* Deleting destructor: sets the vtable, destroys the embedded member arrays, runs the base destructor and frees on request. */
extern "C" {
extern unsigned char D_08A7C9D8[];
extern void func_088D31D0(void *, int);
extern void func_088D2E7C(void *, int);
extern void func_08992F04(void *);
extern void func_08A7BA60(void);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_08A775D8(void *, int);
}

struct ObjC7E0 {
    void *vtbl;
    char sub[0x10];

    static void destroy(ObjC7E0 *object, int flags)
    {
        if (object) {
            object->vtbl = D_08A7C9D8;
            char *m440 = (char *)object + 0x440;
            char *m430 = (char *)object + 0x430;
            if (m440) {
                func_0899372C((char *)object + 0x458, 1, 4, (void *)func_088D31D0, 0, 0);
                func_0899372C(m440, 6, 4, (void *)func_088D2E7C, 0, 0);
            }
            if (m430) {
                func_088D31D0((char *)object + 0x43C, 2);
                func_0899372C(m430, 3, 4, (void *)func_088D2E7C, 0, 0);
            }
            if ((char *)object + 4) {
                char *m3FC = (char *)object + 0x3FC;
                char *m3F8 = (char *)object + 0x3F8;
                if (m3FC) {
                    func_0899372C((char *)object + 0x410, 4, 4, (void *)func_088D31D0, 0, 0);
                    func_0899372C(m3FC, 5, 4, (void *)func_088D2E7C, 0, 0);
                }
                if (m3F8) {
                    func_088D2E7C(m3F8, 2);
                }
                if ((char *)object + 0x3E0) {
                    func_0899372C((char *)object + 0x3E0, 3, 4, (void *)func_088D2E7C, 0, 0);
                }
                func_0899372C((char *)object + 0x20, 0x10, 0x3C, (void *)func_08A7BA60, 0, 0);
                func_0899372C((char *)object + 0x14, 3, 4, (void *)func_088D31D0, 0, 0);
                func_0899372C((char *)object + 4, 4, 4, (void *)func_088D2E7C, 0, 0);
            }
            func_08A775D8(object, 0);
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_08A7C7E0(ObjC7E0 *object, int flags)
{
    ObjC7E0::destroy(object, flags);
}
