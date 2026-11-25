/* If the scale is not 1.0: scales the three basis vectors by it, calls the owner's refresh through its table entry and updates the transform. */
typedef struct Ent3491C { short adj; short pad; void (*fn)(void *); } Ent3491C;
typedef struct Own3491C { char pad[0xF0]; Ent3491C e; } Own3491C;
typedef struct S3491C { char pad0[0x544]; Own3491C *own; char pad1[0x560 - 0x548]; float scale; } S3491C;
extern void func_089A3A44(void *, void *, float);
extern void func_08831994(void *);
void func_08B3491C(S3491C *self)
{
    float s = self->scale;
    if (s != 1.0f) {
        float a = s;
        float b = s;
        func_089A3A44(self, self, s);
        func_089A3A44((char *)self + 0x10, (char *)self + 0x10, a);
        func_089A3A44((char *)self + 0x20, (char *)self + 0x20, b);
        {
            Ent3491C *e = &self->own->e;
            e->fn((char *)self + e->adj);
        }
        func_08831994(self);
    }
}
