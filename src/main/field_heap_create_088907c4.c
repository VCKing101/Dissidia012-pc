/* Allocates the field's main memory block and creates its heap. */
typedef struct FieldMemory {
    int block;
    int heap;
} FieldMemory;

extern char D_089CA720[];

extern int func_0888204C(int size);
extern int func_0885C40C(int block, int size, const char *name, int align);

void func_088907C4(FieldMemory *self)
{
    int size = 0x275800;

    self->block = func_0888204C(size);
    self->heap = func_0885C40C(self->block, size, D_089CA720, 0x10);
}
