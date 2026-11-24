// Destructor shell: sets the vtable, destroys the two owned objects at +0x70 and +0x74, optionally frees self.
extern "C" {
extern char D_08AD75B8[];
void func_08AD1F8C(void *p);
void func_08AD2F38(void *self);
}

struct Sub {
    virtual void v0(unsigned char flag);
};

struct Obj {
    char d0[0x64];
    int f64;
    int f68;
    char d6c[4];
    Sub *s70;
    void *p74;
    char d78[0xC];
    unsigned int f84;
    char d88[0x70];
    void *vt;
};

extern "C" void func_08AD2A10(Obj *o, unsigned int fl) {
    if (o != 0) {
        o->vt = D_08AD75B8;
        if (o->s70 != 0) {
            o->s70->v0((o->f84 & 4) != 0);
            func_08AD1F8C(o->s70);
            o->s70 = 0;
        }
        if (o->p74 != 0) {
            func_08AD1F8C(o->p74);
            o->p74 = 0;
        }
        o->f64 = 0;
        o->f68 = 0;
        if (fl & 1) {
            func_08AD2F38(o);
        }
    }
}
