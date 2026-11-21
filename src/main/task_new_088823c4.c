/* A task scheduler (only its heap is read here). */
typedef struct TaskScheduler {
    void *heap;                        /* +0x00 */
} TaskScheduler;

extern void *func_0885C6CC(void *heap, int size, int align, int tag, int one, int from_end); /* heap_alloc */
extern void func_088822BC(void *task, TaskScheduler *scheduler, int priority, int order, void (*fn)(void));
extern void func_08882790(TaskScheduler *scheduler, void *task);

/* task_new: allocate a 0x4C-byte task from the scheduler's heap and register it; the name
 * is not used in this build. Returns the task, or 0. The three arguments are kept in the
 * frame across the allocation. */
void *func_088823C4(TaskScheduler *scheduler, int priority, int order, void (*fn)(void), const char *name)
{
    void (*kept_fn[1])(void);
    int kept_order[1];
    int kept_priority[1];
    void *task;
    void *result;

    kept_fn[0] = fn;
    kept_order[0] = order;
    kept_priority[0] = priority;
    void *heap = scheduler->heap;
    int one = 1;
    void *block = func_0885C6CC(heap, 0x4C, 0, 0, one, 0);
    result = 0;
    task = block;
    fn = kept_fn[0];
    order = kept_order[0];
    priority = kept_priority[0];
    if (task != 0) {
        func_088822BC(task, scheduler, priority, order, fn);
        result = task;
    }
    func_08882790(scheduler, result);
    return result;
}
