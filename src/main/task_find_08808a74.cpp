extern "C" {
struct Task;
extern Task *D_089D2800;
struct Task {
    char pad0[0x18];
    int owner;
    char pad1C[0x10];
    short state;
    char pad2E[0x3E];
    Task *next;
    char pad70[0x10];
    int key;
    static Task *after(Task *task) { return task == 0 ? D_089D2800 : task->next; }
    static unsigned char running(Task *task) { return task->state == 1; }
};
Task *func_08808A74(int owner, int key)
{
    Task *task = 0;
    while ((task = Task::after(task)) != 0) {
        if (task->owner == owner && Task::running(task) && task->key == key) {
            return task;
        }
    }
    return 0;
}
}
