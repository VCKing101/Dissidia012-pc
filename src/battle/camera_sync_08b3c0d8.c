/* Copies the rig's matrices and vectors into the global camera block when its flag at +0x8BC is set, then refreshes the camera. */
struct Rig {
    char m0[0x40];
    char v40[0x10];
    char v50[0x10];
    char v60[0x10];
    char m70[0x40];
    char vB0[0x10];
    float f[4];
    char mD0[0x40];
    char pad110[0x8BC - 0x110];
    unsigned char on;
};
struct Cam { char d0[0x720]; struct Rig rig; };
extern void func_089A3EFC(void *dst, void *src);
extern void func_089A3868(void *dst, void *src);
extern void func_08866BA8(struct Cam *c);
extern struct Cam *D_089D5EC0;

void func_08B3C0D8(struct Rig *r)
{
    if (r->on == 1) {
        struct Cam *c = D_089D5EC0;
        struct Rig *d = &c->rig;
        float *q = (float *)((char *)c + 0x7E0);
        func_089A3EFC(d->m0, r->m0);
        func_089A3868(d->v40, r->v40);
        func_089A3868(d->v50, r->v50);
        func_089A3868(d->v60, r->v60);
        func_089A3EFC(d->m70, r->m70);
        func_089A3868(d->vB0, r->vB0);
        q[0] = r->f[0];
        q[1] = r->f[1];
        q[2] = r->f[2];
        q[3] = r->f[3];
        func_089A3EFC(q + 4, r->mD0);
        func_08866BA8(D_089D5EC0);
    }
}
