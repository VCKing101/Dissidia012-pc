#include "main/task_runtime.h"

struct MainTaskListAccessors {
    static MainTask *next(MainTaskOwner *owner, MainTask *cursor) {
        MainTask *result;
        if (!cursor) result = owner->field_0004;
        else result = cursor->field_0034;
        return result;
    }
    static MainTask *&previous(MainTask *item) { return item->field_0038; }
    static MainTask *&following(MainTask *item) { return item->field_0034; }
    static MainTask *last(MainTaskOwner *owner) { return owner->field_0008; }
    static void increment_count(MainTaskOwner *owner) { owner->field_000C++; }
    static void prepend(MainTaskOwner *owner, MainTask *item) {
        MainTask *head = owner->field_0004;
        if (!head) {
            owner->field_0008 = item;
            owner->field_0004 = item;
            item->field_0034 = 0;
            item->field_0038 = 0;
        } else {
            previous(head) = item;
            item->field_0038 = 0;
            item->field_0034 = owner->field_0004;
            owner->field_0004 = item;
        }
        increment_count(owner);
    }
    static void append(MainTaskOwner *owner, MainTask *item) {
        MainTask *tail = owner->field_0008;
        if (!tail) {
            owner->field_0008 = item;
            owner->field_0004 = item;
            item->field_0034 = 0;
            item->field_0038 = 0;
        } else {
            following(tail) = item;
            MainTask *tail = owner->field_0008;
            item->field_0034 = 0;
            item->field_0038 = tail;
            owner->field_0008 = item;
        }
        increment_count(owner);
    }
    static void insert_after(MainTaskOwner *owner, MainTask *previous_item, MainTask *item) {
        item->field_0038 = previous_item;
        item->field_0034 = previous_item->field_0034;
        previous(previous_item->field_0034) = item;
        previous_item->field_0034 = item;
        owner->field_000C++;
    }
};

extern "C" void func_08882790(MainTaskOwner *owner, MainTask *item) {
    MainTask *previous = 0;
    MainTask *cursor = 0;
    MainTask *following;
    while ((cursor = (following = MainTaskListAccessors::next(owner,cursor)))) {
        if (item->field_002C < cursor->field_002C) break;
        previous = following;
    }
    if (following && owner->field_0014 == following) owner->field_0014 = item;
    if (!previous) MainTaskListAccessors::prepend(owner,item);
    else if (previous == MainTaskListAccessors::last(owner)) MainTaskListAccessors::append(owner,item);
    else MainTaskListAccessors::insert_after(owner,previous,item);
}
