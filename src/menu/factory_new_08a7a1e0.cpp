/* Factory: new (heap) T for a 0x38-byte class with members at +4 and +0x14.
   Retail keeps the new-expression's null result in the frame (0($sp)). */
extern "C" void *func_0885C6CC(void *heap, unsigned size, int align, int offset, int kind, int flags);

struct Object08A7A1E0 {
    void *vtable;
    char field_04[0x10];
    char field_14[0x24];
};

extern "C" {

extern char D_08A784E8[];
void func_08A74260(void *member);
void func_08A72D18(void *member);

Object08A7A1E0 *func_08A7A1E0(void *heap)
{
    Object08A7A1E0 *none = 0;
    Object08A7A1E0 *result[1];
    result[0] = none;
    int one = 1;
    Object08A7A1E0 *self = (Object08A7A1E0 *)func_0885C6CC(heap, 0x38, 0, 0, one, 0);
    Object08A7A1E0 *made = result[0];
    if (self != 0) {
        self->vtable = D_08A784E8;
        func_08A74260(self->field_04);
        func_08A72D18(self->field_14);
        made = self;
    }
    return made;
}

}
