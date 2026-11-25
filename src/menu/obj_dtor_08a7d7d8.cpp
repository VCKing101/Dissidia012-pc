/* Deleting destructor: sets the vtable, destroys the embedded member arrays, runs the base destructor and frees on request. */
extern "C" {
extern unsigned char D_08A7DCA0[];
extern void func_088D31D0(void *, int);
extern void func_088D2E7C(void *, int);
extern void func_08992F04(void *);
extern void func_08A7CAA4(void);
extern void func_0899372C(void *, unsigned int, unsigned int, void *, int, int);
extern void func_08A775D8(void *, int);
}

struct ObjD7D8 {
    void *vtbl;
    char sub[0x10];

    static void destroy(ObjD7D8 *object, int flags)
    {
        if (object) {
            object->vtbl = D_08A7DCA0;
            char *m1E4 = (char *)object + 0x1E4;
            if (m1E4 && (char *)object + 0x1E4) {
                if ((char *)object + 0x1FC) {
                    func_088D2E7C((char *)object + 0x1FC, 2);
                }
                func_0899372C((char *)object + 0x1F8, 1, 4, (void *)func_088D31D0, 0, 0);
                func_0899372C(m1E4, 5, 4, (void *)func_088D2E7C, 0, 0);
            }
            if ((char *)object + 4) {
                if ((char *)object + 0x1DC) {
                    func_088D2E7C((char *)object + 0x1DC, 2);
                }
                if ((char *)object + 0x1D0) {
                    func_0899372C((char *)object + 0x1D4, 2, 4, (void *)func_088D31D0, 0, 0);
                    func_088D2E7C((char *)object + 0x1D0, 2);
                }
                if ((char *)object + 0x104) {
                    func_0899372C((char *)object + 0x108, 10, 0x14, (void *)0x08A7CA48, 0, 0);
                    func_088D2E7C((char *)object + 0x104, 2);
                }
                func_0899372C((char *)object + 0xC, 2, 0x7C, (void *)func_08A7CAA4, 0, 0);
                func_088D31D0((char *)object + 8, 2);
                func_088D2E7C((char *)object + 4, 2);
            }
            func_08A775D8(object, 0);
            if (flags & 1) func_08992F04(object);
        }
    }
};

extern "C" void func_08A7D7D8(ObjD7D8 *object, int flags)
{
    ObjD7D8::destroy(object, flags);
}
