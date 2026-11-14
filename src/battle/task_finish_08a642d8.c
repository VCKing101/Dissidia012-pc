typedef struct TaskState642D8 {
    char pad_000[0x30];
    int state;
} TaskState642D8;

typedef struct TaskOwner642D8 {
    unsigned char active;
    char pad_001[0x1F];
    char task[0x4];
} TaskOwner642D8;

extern TaskState642D8 *func_08AB326C(void *task);
extern void *func_08AB327C(void *task);
extern int func_08AB0EC4(void *handle);

int func_08A642D8(TaskOwner642D8 *self)
{
    if (!self->active) {
        return -1;
    }
    if (func_08AB326C(self->task)->state == 1) {
        return 0;
    }
    return func_08AB0EC4(func_08AB327C(self->task));
}
