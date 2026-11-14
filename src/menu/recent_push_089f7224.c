/* Puts the id into the first free of three recent slots, or drops the oldest; returns the slot used. */
typedef struct RecentSlot {
    signed char field_00;
    unsigned char field_01;
    char pad_02[0x2];
} RecentSlot;

typedef struct RecentState {
    char pad_0000[0x2E70];
    RecentSlot field_2E70[3];
} RecentState;

typedef struct RecentOwner {
    char pad_000[0x14C];
    RecentState *field_14C;
} RecentOwner;

int func_089F7224(RecentOwner *self, signed char id)
{
    int slot = 0;
    int i;
    for (i = 0; i < 3; i++) {
        if (self->field_14C->field_2E70[i].field_00 == -1) {
            self->field_14C->field_2E70[i].field_00 = id;
            self->field_14C->field_2E70[i].field_01 = 0;
            slot = i;
            break;
        }
    }
    if (i == 3) {
        for (i = 0; i < 2; i++) {
            self->field_14C->field_2E70[i].field_00 = self->field_14C->field_2E70[i + 1].field_00;
            self->field_14C->field_2E70[i].field_01 = self->field_14C->field_2E70[i + 1].field_01;
        }
        self->field_14C->field_2E70[i].field_00 = id;
        self->field_14C->field_2E70[i].field_01 = 0;
        slot = i;
    }
    return slot;
}
