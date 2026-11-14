/* Picks an entry to swap: the first pick is remembered, the second swaps the two 16-byte entries; returns 1 on a swap. */
typedef struct SwapEntry {
    char data[0x10];
} SwapEntry;

typedef struct SwapState {
    char pad_0000[0x7188];
    SwapEntry *field_7188;
} SwapState;

typedef struct SwapOwner {
    char pad_00[0x5C];
    SwapState *field_5C;
} SwapOwner;

extern void *func_08935524(void *dst, const void *src, int size);
extern void func_08A4DE54(SwapOwner *self);

int func_08A4DFD8(SwapOwner *self, SwapEntry *entry)
{
    SwapEntry saved;
    if (entry == 0) {
        self->field_5C->field_7188 = entry;
        func_08A4DE54(self);
        return 0;
    }
    if (self->field_5C->field_7188 == 0) {
        self->field_5C->field_7188 = entry;
        return 0;
    }
    if (entry != self->field_5C->field_7188) {
        func_08935524(&saved, entry, 0x10);
        func_08935524(entry, self->field_5C->field_7188, 0x10);
        func_08935524(self->field_5C->field_7188, &saved, 0x10);
    }
    self->field_5C->field_7188 = 0;
    func_08A4DE54(self);
    return 1;
}
