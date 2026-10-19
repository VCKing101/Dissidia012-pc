#include "main_task_iteration.hpp"
extern "C" void func_088822E0(MainTask *);

struct MainTaskUnlinkOperations {
    static MainTask *&following(MainTask *task) { return task->field_0034; }
    static MainTask *&previous(MainTask *task) { return task->field_0038; }
    static MainTask **following_slot(MainTask *task) { return &task->field_0034; }
    static MainTask **previous_slot(MainTask *task) { return &task->field_0038; }
    static void unlink(MainTaskOwner *owner, MainTask *task) {
        MainTask *previous_task = task->field_0038;
        MainTask *&previous_next = previous_task ? previous_task->field_0034 : owner->field_0004;
        previous_next = task->field_0034;
        MainTask *next_task = task->field_0034;
        MainTask *&next_previous = next_task ? next_task->field_0038 : owner->field_0008;
        next_previous = task->field_0038;
        task->field_0038 = 0;
        task->field_0034 = 0;
        owner->field_000C--;
    }
};
extern "C" void func_088828BC(MainTaskOwner *owner, MainTask *task) {
    if (task == owner->field_0014) owner->field_0014 = MainTaskIteration::next(owner,task);
    MainTaskUnlinkOperations::unlink(owner,task);
    func_088822E0(task);
}
