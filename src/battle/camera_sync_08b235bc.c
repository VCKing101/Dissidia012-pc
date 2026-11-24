/* Copies the source rig's matrices and vectors into the global camera block when its flag at +0x1A0 is set, then refreshes the camera. */
struct Src {
    char d0[0x40];
    char m40[0x40];
    char v80[0x10];
    char v90[0x10];
    char vA0[0x10];
    char mB0[0x40];
    char vF0[0x10];
    float f[4];
    char m110[0x40];
    char pad150[0x1A0 - 0x150];
    unsigned char on;
};
struct Rig {
    char m0[0x40];
    char v40[0x10];
    char v50[0x10];
    char v60[0x10];
    char m70[0x40];
    char vB0[0x10];
};
struct Cam { char d0[0x720]; struct Rig rig; };
extern void func_089A3EFC(void *dst, void *src);
extern void func_089A3868(void *dst, void *src);
extern void func_08866BA8(struct Cam *c);
extern struct Cam *D_089D5EC0;

void func_08B235BC(struct Src *r)
{
    if (r->on == 1) {
        struct Cam *c = D_089D5EC0;
        struct Rig *d = &c->rig;
        float *q = (float *)((char *)c + 0x7E0);
        func_089A3EFC(d->m0, r->m40);
        func_089A3868(d->v40, r->v80);
        func_089A3868(d->v50, r->v90);
        func_089A3868(d->v60, r->vA0);
        func_089A3EFC(d->m70, r->mB0);
        func_089A3868(d->vB0, r->vF0);
        q[0] = r->f[0];
        q[1] = r->f[1];
        q[2] = r->f[2];
        q[3] = r->f[3];
        func_089A3EFC(q + 4, r->m110);
        func_08866BA8(D_089D5EC0);
    }
}
