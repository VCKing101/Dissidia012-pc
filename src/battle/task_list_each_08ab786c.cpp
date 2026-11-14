typedef struct BattleTask BattleTask;

typedef struct TaskList {
    unsigned char active;
    int count;
    BattleTask **tasks;
} TaskList;

extern "C" {

extern int func_08AB6A80(BattleTask *task);

/* Run func_08AB6A80 on every task of an active list. */
int func_08AB786C(TaskList *self)
{
    if (self->active == 0) {
        return -1;
    }
    for (int i = 0; i < self->count; i++) {
        func_08AB6A80(self->tasks[i]);
    }
    return 0;
}

}
