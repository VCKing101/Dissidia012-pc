#include "main/drawing_flag_accessors.h"

struct MainDrawingPoolRecord : MainDrawingRecordPrefix {
    unsigned char unknown_0030[0x0C];
    MainDrawingPoolRecord *next_003C;
    MainDrawingPoolRecord *previous_0040;
};
struct MainDrawingPoolList {
    MainDrawingPoolRecord *head;
    MainDrawingPoolRecord *tail;
    unsigned int count;
};
struct MainDrawingPoolSlot {
    MainDrawingPoolRecord *record;
    MainDrawingPoolSlot *next;
};
struct MainDrawingPoolState {
    unsigned char unknown_0000[0x9178];
    MainDrawingPoolSlot *free_head;
};
struct MainDrawingPoolOrigin { short first; short second; };
extern "C" void *D_089D7830;
extern "C" void func_088D99DC(MainDrawingRecordPrefix *, void *, int, int, int);

struct MainDrawingPoolOperations {
    static MainDrawingPoolState *state(void *runtime) {
        return reinterpret_cast<MainDrawingPoolState *>(static_cast<unsigned char *>(runtime)+0x3374);
    }
    static MainDrawingPoolRecord **previous_slot(MainDrawingPoolRecord *record) {
        /* Integer address calculation is defined for a null value; dereference
         * is performed only on the guarded, nonempty-list path. */
        return reinterpret_cast<MainDrawingPoolRecord **>(reinterpret_cast<unsigned long>(record)+0x40);
    }
    static void prepend(MainDrawingPoolList *owner, MainDrawingPoolRecord *record) {
        MainDrawingPoolRecord *head = owner->head;
        MainDrawingPoolRecord **previous_head = previous_slot(head);
        if (!head) {
            owner->tail = record;
            owner->head = record;
            record->next_003C = 0;
            record->previous_0040 = 0;
        } else {
            *previous_head = record;
            record->previous_0040 = 0;
            record->next_003C = owner->head;
            owner->head = record;
        }
        owner->count++;
    }
};

extern "C" void *func_088DA954(MainDrawingPoolList *owner, int index,
                              MainDrawingPoolOrigin origin, unsigned char flags) {
    MainDrawingPoolState *pool = MainDrawingPoolOperations::state(D_089D7830);
    MainDrawingPoolSlot *slot = pool->free_head;
    MainDrawingPoolRecord *record = slot->record;
    slot->record = 0;
    MainDrawingPoolSlot *next_slot = pool->free_head;
    next_slot = next_slot->next;
    pool->free_head = next_slot;
    func_088D99DC(record,owner,index,origin.first,origin.second);
    if (flags) {
        MainDrawingFlagAccessors::set_flag(record,0x100);
        MainDrawingFlagAccessors::set_flag(record,0x1000);
    }
    MainDrawingPoolOperations::prepend(owner,record);
    return record;
}
