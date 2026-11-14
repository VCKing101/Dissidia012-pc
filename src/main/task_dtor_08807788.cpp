/* Deleting destructor of a task kept on the global task list: unlinks it,
   releases its resources unless they are shared, and frees it. */
struct Task08807788 {
    char pad_00[0x10];
    unsigned int flags;
    char pad_14[0x26 - 0x14];
    short slot;
    char pad_28[0x6C - 0x28];
    Task08807788 *next;
    void *vtable;

    static bool shared(Task08807788 *t) { return (t->flags & 0x20) != 0; }
};

struct TaskList08807788 {
    Task08807788 *head;
    Task08807788 *tail;

    void remove(Task08807788 *task)
    {
        Task08807788 *head = this->head;
        Task08807788 *tail = this->tail;
        if (head != 0 && tail != 0) {
            Task08807788 *next = task->next;
            Task08807788 *prev;
            if (task == this->head) {
                this->head = next;
                prev = 0;
            } else {
                prev = this->head;
                while (prev->next != task) {
                    prev = prev->next;
                }
                prev->next = next;
            }
            if (task == this->tail) {
                this->tail = prev;
            }
            task->next = 0;
        }
    }
};

extern "C" {

extern TaskList08807788 D_089D2800;
extern int D_089C6C40;

void func_0880826C(Task08807788 *self);
void func_0880AFA0(int slot);
void func_08807A1C(Task08807788 *self);

}

struct TaskDtor08807788 {
    static void rest(Task08807788 *self, unsigned int flags)
    {
        if (!Task08807788::shared(self)) {
            func_0880826C(self);
            func_0880AFA0(self->slot);
        }
        if (flags & 1) {
            func_08807A1C(self);
        }
    }
};

extern "C" {

void func_08807788(Task08807788 *self, unsigned int flags)
{
    if (self != 0) {
        self->vtable = &D_089C6C40;
        D_089D2800.remove(self);
        TaskDtor08807788::rest(self, flags);
    }
}

}
