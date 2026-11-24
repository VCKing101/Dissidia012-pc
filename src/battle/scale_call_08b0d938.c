/* If the object has a target, forwards it and a rate (signed byte at +0x12 of the record times 0.01) to two helpers. */
extern void func_088A3D30(void *t, int a, int b, int c);
extern void func_0889F8D4(float rate, void *t, int a);
typedef struct { char pad[0x12]; signed char rate; } Rec;
typedef struct { int pad0; char *target; int a; Rec *rec; } Obj;
void func_08B0D938(Obj *self) {
    if (self->target != 0) {
        signed char c = self->rec->rate;
        float rate = (float)c * 0.01f;
        func_088A3D30(self->target, self->a, 0, 0);
        func_0889F8D4(rate, self->target + 4, 0);
    }
}
