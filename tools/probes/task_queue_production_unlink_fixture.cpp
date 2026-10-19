#include <cassert>
#include <cstddef>
#include <cstdio>
#include "../../src/main/task_unlink.cpp"

static_assert(sizeof(TaskQueueAddress) == sizeof(MainTask *), "address width");
static_assert(offsetof(MainTask, field_0034) == sizeof(MainTask) - sizeof(TaskQueueLinkSuffix), "following offset");
static_assert(offsetof(MainTask, field_0038) == sizeof(MainTask) - sizeof(TaskQueueLinkSuffix) + sizeof(MainTask *), "previous offset");

static MainTaskOwner *active_owner;
static MainTask *finalized;
static int finalizer_calls;
static int expected_count;
extern "C" void func_088822E0(MainTask *task) {
    assert(task && !task->field_0034 && !task->field_0038);
    assert(active_owner->field_000C == expected_count);
    finalized = task;
    ++finalizer_calls;
}

static void remove_case(int count, int remove, bool pending_is_removed) {
    MainTaskOwner owner = {};
    MainTask tasks[3] = {};
    MainTask auxiliary = {};
    for (int i = 0; i < count; ++i) {
        tasks[i].field_0038 = i ? &tasks[i - 1] : 0;
        tasks[i].field_0034 = i + 1 < count ? &tasks[i + 1] : 0;
        tasks[i].field_003C = &auxiliary;
        tasks[i].field_0040 = &auxiliary;
    }
    owner.field_0004 = &tasks[0];
    owner.field_0008 = &tasks[count - 1];
    owner.field_000C = count;
    owner.field_0014 = pending_is_removed ? &tasks[remove] : 0;
    MainTask *expected_pending = pending_is_removed ? tasks[remove].field_0034 : 0;
    active_owner = &owner;
    expected_count = count - 1;
    finalized = 0;
    finalizer_calls = 0;
    func_088828BC(&owner, &tasks[remove]);
    assert(finalizer_calls == 1 && finalized == &tasks[remove]);
    assert(owner.field_0014 == expected_pending);
    assert(owner.field_000C == count - 1);
    MainTask *previous = 0;
    MainTask *cursor = owner.field_0004;
    for (int i = 0; i < count; ++i) {
        if (i == remove) continue;
        assert(cursor == &tasks[i] && cursor->field_0038 == previous);
        previous = cursor;
        cursor = cursor->field_0034;
    }
    assert(!cursor && owner.field_0008 == previous);
    for (int i = 0; i < count; ++i) {
        assert(tasks[i].field_003C == &auxiliary && tasks[i].field_0040 == &auxiliary);
    }
    if (!owner.field_000C) {
        assert(!owner.field_0004 && !owner.field_0008);
        assert(!MainTaskIteration::next(&owner, 0));
    }
}

int main() {
    MainTaskOwner empty = {};
    assert(!MainTaskIteration::next(&empty, 0));
    // Native unlink requires an existing nonnull task; empty/null unlink is
    // deliberately outside the contract rather than introducing a new guard.
    for (int pending = 0; pending < 2; ++pending) {
        remove_case(1, 0, pending != 0);
        for (int position = 0; position < 3; ++position) {
            remove_case(3, position, pending != 0);
        }
    }
    std::puts("PASS: empty boundary, single/head/tail/interior unlink, pending cursor, finalizer ordering and auxiliary preservation (8 removal cases)");
}
