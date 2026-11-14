/* Looks up the current slot (clamped at 0) and returns its value.
   Retail keeps both lookups in the frame; one-element arrays reproduce that. */
typedef struct PickEntry {
    char pad_00[0x8];
    short field_08;
    char pad_0A[0x2];
} PickEntry;

typedef struct PickState {
    char pad_000[0x5FC];
    int field_5FC;
} PickState;

typedef struct PickOwner {
    char pad_00[0x28];
    PickState *field_28;
} PickOwner;

extern PickEntry *func_08A85ECC(PickOwner *self);
extern int func_08A7F280(PickOwner *self, int value);
extern int func_08A7F104(PickOwner *self, int kind, int value);

int func_08A81188(PickOwner *self)
{
    int slot[1];
    int value[1];
    PickEntry *entries = func_08A85ECC(self);
    value[0] = func_08A7F280(self, self->field_28->field_5FC);
    slot[0] = func_08A7F104(self, 2, value[0]);
    if (slot[0] < 0) {
        slot[0] = 0;
    }
    value[0] = entries[slot[0]].field_08;
    return value[0];
}
