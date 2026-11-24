/* Constructor: sets the vtable, constructs the embedded members and clears the state fields. */
extern "C" {
extern unsigned char D_08A7DCA0[];
extern unsigned char D_08A7DC88[];
extern void func_08993560(void *, int, int, void *);
extern void func_08A73C38(void *);
extern void func_08A76574(void *);
extern void func_088D2E70(void);
extern void func_088D31C4(void);
}

struct ObjCB48 {
    void *vtbl;
    char sub[0x1E0];
    char arrA[0x14];
    char arrB[4];
    char sub2[0x18];
    void *f214;
    ObjCB48 *f218;
    int f21C;
    int f220;
    float f224;
    int f228;
    int f22C;

    static ObjCB48 *construct(ObjCB48 *self, void *vt, void *vt2)
    {
        self->vtbl = vt;
        func_08A76574(self->sub);
        func_08993560(self->arrA, 5, 4, (void *)func_088D2E70);
        func_08993560(self->arrB, 1, 4, (void *)func_088D31C4);
        func_08A73C38(self->sub2);
        self->f214 = vt2;
        self->f218 = self;
        self->f21C = -1;
        self->f228 = 0;
        self->f22C = 0;
        self->f220 = 0;
        self->f224 = 0.0f;
        return self;
    }
};

extern "C" ObjCB48 *func_08A7CB48(ObjCB48 *self)
{
    return ObjCB48::construct(self, D_08A7DCA0, D_08A7DC88);
}
