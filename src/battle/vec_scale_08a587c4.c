struct V4 { float x, y, z, w; };
struct Tgt { char pad[0x84]; float f84; };
struct M { char pad[0x90]; char a90[0xA0]; char a130[0x78]; float f1a8; char pad2[0x1F0]; struct Tgt *p39c; char pad3[8]; float *p3a8; };
extern float D_08A58980;
extern struct V4 D_09AF84C0;
extern void *func_08835778(void *p);
extern char *func_088BF4CC(void *p, int id);
extern float func_0882FC20(void *p, int a);
extern float func_089A46DC(float f);
extern float func_08838300(void *p);
extern void func_089A3A44(struct V4 *out, float f, void *p);
void func_08A587C4(struct V4 *out, int unused, struct M *self)
{
    struct V4 r;
    if (D_08A58980 <= *self->p3a8) {
        *out = D_09AF84C0;
    } else {
        float t = self->p39c->f84;
        char *e = func_088BF4CC(func_08835778(self), 0x3B);
        float a, z;
        if (e) { t = t + (float)*(short *)(e + 4) * 0.001f; }
        a = func_0882FC20(self->a130, -1);
        z = func_089A46DC(self->f1a8 / a);
        t = t * z;
        func_089A3A44(&r, t * func_08838300(self), self->a90);
        *out = r;
    }
}
