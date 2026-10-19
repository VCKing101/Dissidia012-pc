/* Nonmatching candidate: explicitly excluded from the overlay build. */
#include "extra/data_install.h"

typedef struct DataInstallEntry {
    u32 unknown_0000;
    u32 unknown_0004;
    u32 flags;
} DataInstallEntry;

extern u32 D_089D5FB4;
extern u32 D_089D5FF0;
extern u32 func_886FA84(u32 context);
extern void func_886FA1C(u32 context);
extern u32 func_8935828(void *destination, u32 value, u32 size);
extern s32 func_8871674(u32 context);
extern DataInstallEntry *func_8871684(u32 context, s32 index);
extern u32 func_88716C8(u32 context, DataInstallEntry *entry);
extern u32 func_886FCDC(u32 context, DataInstallEntry *entry);
extern void func_886FAC0(u32 context, s32 count, s32 index, u64 offset);
extern void func_886FAA0(u32 context, s32 count, s32 argument, u32 total);

void func_089DD99C(DataInstallOwner *owner, s32 argument) {
    s32 count = 0;
    u64 offset = (func_886FA84(D_089D5FB4) + 0x1FFF) & ~0x1FFFU;
    s32 index;
    u32 total = 0;

    func_8935828(owner->state->entries, 0, 0x3F48);
    owner->state->entry_count = 0;
    owner->state->current_entry_index = 0;
    func_886FA1C(D_089D5FB4);
    index = 0;

    while (index < func_8871674(D_089D5FF0)) {
        DataInstallEntry *entry;
        u32 category;
        u32 size;

        entry = func_8871684(D_089D5FF0, index);
        if (!entry) {
            break;
        }
        category = entry->flags & 0xF0000000U;
        if (category && argument >= (s32)(category >> 28)) {
            total += func_88716C8(D_089D5FF0, entry);
            size = func_886FCDC(D_089D5FB4, entry);
            if (size <= owner->state->buffer_capacity_bytes) {
                owner->state->entries[count] = (u32)entry;
                func_886FAC0(D_089D5FB4, count, index, offset);
                offset += size;
                count++;
            }
        }
        if (count >= 0xFD2) {
            return;
        }
        index++;
    }

    func_886FAA0(D_089D5FB4, count, argument, total);
    owner->state->entry_count = count;
}
