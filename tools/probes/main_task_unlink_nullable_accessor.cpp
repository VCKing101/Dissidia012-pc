#include "main_task_iteration.hpp"
extern "C" void func_088822E0(MainTask *);

struct MainTaskUnlinkOperations {
    static MainTask *&following(MainTask *task) { return task->field_0034; }
    static MainTask *&previous(MainTask *task) { return task->field_0038; }
    static MainTask **following_slot(MainTask *task) { return &task->field_0034; }
    static MainTask **previous_slot(MainTask *task) { return &task->field_0038; }
    static MainTask *&following_or_head(MainTaskOwner *owner, MainTask *task) {
        return task ? task->field_0034 : owner->field_0004;
    }
    static MainTask *&previous_or_tail(MainTaskOwner *owner, MainTask *task) {
        return task ? task->field_0038 : owner->field_0008;
    }
    static void unlink(MainTaskOwner *owner, MainTask *task) {
        MainTask *previous_task = task->field_0038;
        MainTask *&previous_next = following_or_head(owner,previous_task);
        previous_next = task->field_0034;
        MainTask *next_task = task->field_0034;
        MainTask *&next_previous = previous_or_tail(owner,next_task);
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
