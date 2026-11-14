/* Frees the slot holding the last placed piece; true when one was freed. */
typedef struct FreeSlotEntry {
    signed char field_00;
    char pad_01[0xB];
} FreeSlotEntry;

typedef struct FreeSlotState {
    char pad_0000[0x19BC];
    int field_19BC;
    char pad_19C0[0x4];
    short field_19C4[(0x19DF - 0x19C4) / 2];
    char pad_19DE;
    unsigned char field_19DF;
    char pad_19E0[0x19E8 - 0x19E0];
    FreeSlotEntry field_19E8[16];
} FreeSlotState;

typedef struct FreeSlotOwner {
    char pad_0000[0x23F0];
    FreeSlotState *field_23F0;
} FreeSlotOwner;

extern void func_08AEC824(FreeSlotOwner *self);

int func_08AEC998(FreeSlotOwner *self)
{
    FreeSlotState *state = self->field_23F0;
    int result = 0;
    if (state->field_19BC != 0) {
        int last = state->field_19BC - 1;
        int i;
        for (i = 0; i < state->field_19DF; i++) {
            if (state->field_19E8[i].field_00 == last) {
                state->field_19E8[i].field_00 = -1;
                self->field_23F0->field_19C4[last] = 0x7FFF;
                func_08AEC824(self);
                result = 1;
                break;
            }
        }
    }
    return result;
}
