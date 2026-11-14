typedef struct BattleTask BattleTask;

typedef struct TaskList {
    unsigned char active;
    int count;
    BattleTask **tasks;
} TaskList;

extern "C" {

extern int func_08AB6DA8(BattleTask *task);

/* Run func_08AB6DA8 on every task of an active list. */
int func_08AB7E50(TaskList *self)
{
    if (self->active == 0) {
        return -1;
    }
    for (int i = 0; i < self->count; i++) {
        func_08AB6DA8(self->tasks[i]);
    }
    return 0;
}

}
