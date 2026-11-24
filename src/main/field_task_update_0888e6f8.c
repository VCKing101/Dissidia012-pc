/* Starts the field update task. */
typedef struct FieldCtl {
    char pad_000[0x100];
    char task[4];
} FieldCtl;

extern char D_089CA5F8[];

extern void func_08865088(void *task, int id, int priority, void (*fn)(void), const char *name);
extern void func_0888EA00(void);

void func_0888E6F8(FieldCtl *self)
{
    func_08865088(self->task, 0, 0, func_0888EA00, D_089CA5F8);
}
