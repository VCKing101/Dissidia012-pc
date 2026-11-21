typedef struct SystemTasks0888221C {
    unsigned char pool[0x2000];
    void *heap;                        /* +0x2000 */
} SystemTasks0888221C;

extern char D_089C9EE8[];              /* "SystemTask" */
extern void *func_08882390(void *base, int size, const char *name);

void func_0888221C(SystemTasks0888221C *self)
{
    self->heap = func_08882390(self, 0x2000, D_089C9EE8);
}
