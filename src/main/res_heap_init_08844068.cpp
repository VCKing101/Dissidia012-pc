/* Creates the resource heap: registers the loader callback, then
   allocates the 260 KB heap block and its allocator. */
struct ResHeap08844068 {
    char *block;
    void *heap;
    int used;
    int count;
};

extern "C" {

extern void *D_089D69A8;
extern char D_089C88AC[];
extern char D_089C88B8[];

void func_08844F84(void);
void *func_08882274(void *system);
void func_088823C4(void *w, int id, int size, void (*callback)(void), char *name);
char *func_088820D8(int size);
void *func_0885C40C(char *block, int size, char *name, int align);

void func_08844068(ResHeap08844068 *self)
{
    func_088823C4(func_08882274(D_089D69A8), -1, 0x17318, func_08844F84, D_089C88AC);
    int size = 0x41000;
    self->block = func_088820D8(size);
    self->heap = func_0885C40C(self->block, size, D_089C88B8, 0x10);
    self->count = 0;
    self->used = 0;
}

}
