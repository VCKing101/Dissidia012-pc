/* Allocates the field's temporary memory block from its heap and creates
   the temporary heap. */
typedef struct FieldMemory {
    int block;
    int heap;
    int tmp_heap;
    int tmp_block;
} FieldMemory;

extern char D_089CA730[];

extern int func_0885C6CC(int heap, int size, int align, int a, int b, int c);
extern int func_0885C40C(int block, int size, const char *name, int align);

void func_08890874(FieldMemory *self)
{
    int size = 0x6B800;

    self->tmp_block = func_0885C6CC(self->heap, size, 0x40, 0, 1, 0);
    self->tmp_heap = func_0885C40C(self->tmp_block, size, D_089CA730, 0x10);
}
