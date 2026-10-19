#include "main_task_iteration.hpp"
extern "C" void func_088822E0(MainTask *);

struct MainTaskUnlinkOperations {
    static MainTask *&following(MainTask *task) { return task->field_0034; }
    static MainTask *&previous(MainTask *task) { return task->field_0038; }
    static MainTask **following_slot(MainTask *task) {
        MainTask *MainTask::*member = &MainTask::field_0034;
        return &(task->*member);
    }
    static MainTask **previous_slot(MainTask *task) {
        MainTask *MainTask::*member = &MainTask::field_0038;
        return &(task->*member);
    }
    static void unlink(MainTaskOwner *owner, MainTask *task) {
        MainTask *previous_task = task->field_0038;
        MainTask *next_task = task->field_0034;
        if (previous_task) *following_slot(previous_task) = next_task;
        else owner->field_0004 = next_task;
        previous_task = task->field_0038;
        next_task = task->field_0034;
        if (next_task) *previous_slot(next_task) = previous_task;
        else owner->field_0008 = previous_task;
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
