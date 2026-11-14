/* Factories: new (heap) T for eight small menu classes (vtable only).
   Each allocates through the heap allocator and runs T's inline constructor.
   Retail keeps the new-expression's null result in the frame (0($sp)) across
   the allocation call instead of an s-register. One-element arrays are never
   promoted by SNC -O2, so `result[1]` reproduces the slot; `one` and `none`
   are the named temporaries the inlined operator new leaves (both `ori`). */
extern "C" void *func_0885C6CC(void *heap, unsigned size, int align, int offset, int kind, int flags);

struct Object {
    void *vtable;
};

extern "C" {

extern char D_08A6EAC8[];
extern char D_08A6DC00[];
extern char D_08A6DCF0[];
extern char D_08A6E840[];
extern char D_08A6DAD0[];
extern char D_08A6E988[];
extern char D_08A6E018[];
extern char D_08A6E138[];

Object *func_08A6E4A0(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x4, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A6EAC8;
        made = self;
    }
    return made;
}

Object *func_08A6E4F0(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0xC, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A6DC00;
        made = self;
    }
    return made;
}

Object *func_08A6E540(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x4, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A6DCF0;
        made = self;
    }
    return made;
}

Object *func_08A6E590(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x4, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A6E840;
        made = self;
    }
    return made;
}

Object *func_08A6E5E0(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x18, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A6DAD0;
        made = self;
    }
    return made;
}

Object *func_08A6E630(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x4, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A6E988;
        made = self;
    }
    return made;
}

Object *func_08A6E680(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x10, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A6E018;
        made = self;
    }
    return made;
}

Object *func_08A6E6D0(void *heap)
{
    int one = 1;
    Object *none = 0;
    Object *result[1];
    result[0] = none;
    Object *self = (Object *)func_0885C6CC(heap, 0x4, 0, 0, one, 0);
    Object *made = result[0];
    if (self != 0) {
        self->vtable = D_08A6E138;
        made = self;
    }
    return made;
}

}
