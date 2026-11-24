/* Allocates 0x270800 bytes with func_0888204C into field 0, then registers it as "TEM::MEMORY" (align 0x10) via func_0885C40C into field 4. */
typedef struct Obj088dfba0 { void *mem; void *pool; } Obj088dfba0;
extern char D_089CC348[];
extern void *func_0888204C(unsigned size);
extern void *func_0885C40C(void *mem, unsigned size, char *name, int align);
void func_088DFBA0(Obj088dfba0 *self)
{
    unsigned size = 0x270800;
    self->mem = func_0888204C(size);
    self->pool = func_0885C40C(self->mem, size, D_089CC348, 0x10);
}
