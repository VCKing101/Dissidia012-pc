/* When the owner's flag 0x40 is set, applies a unit-scaled value from the target's virtual getter, then passes the second getter's result on. */
typedef struct FPmf { short adj; short pad; float (*fn)(void *); } FPmf;
typedef struct IPmf { short adj; short pad; int (*fn)(void *); } IPmf;
typedef struct Tbl { char p0[0x18]; FPmf a; char p1[0x40 - 0x20]; IPmf b; } Tbl;
typedef struct Tgt { char p0[0x2C]; Tbl *t; } Tgt;
typedef struct Own { char p0[0x7C]; unsigned int flags; } Own;
typedef struct S { char p0[0xC0]; Own *own; char p1[0xCC - 0xC4]; void *obj; Tgt *tgt; } S;
extern void func_088A4760(void *o, float a, float b, float c, float d);
extern void func_088A4894(void *o, int v);
void func_088A7664(S *self)
{
    if (self->own->flags & 0x40) {
        void *obj = self->obj;
        Tgt *t = self->tgt;
        FPmf *a = &t->t->a;
        func_088A4760(obj, 1.0f, 1.0f, 1.0f, a->fn((char *)t + a->adj));
        {
            void *obj2 = self->obj;
            Tgt *t2 = self->tgt;
            IPmf *b = &t2->t->b;
            func_088A4894(obj2, b->fn((char *)t2 + b->adj));
        }
    }
}
