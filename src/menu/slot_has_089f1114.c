/* Whether one of the nine slots is an active kind-2 entry of type 3 with this id. */
typedef struct SlotEntry {
    unsigned char field_00;
    char pad_01[0x3];
    unsigned int field_04;
    char pad_08[0x4];
    unsigned char field_0C;
    char pad_0D;
    short field_0E;
    char pad_10[0x84 - 0x10];
} SlotEntry;

typedef struct SlotState {
    char pad_0000[0x29CC];
    SlotEntry field_29CC[9];
} SlotState;

typedef struct SlotOwner {
    char pad_000[0x14C];
    SlotState *field_14C;
} SlotOwner;

int func_089F1114(SlotOwner *self, short id)
{
    int result = 0;
    int i;
    for (i = 0; i < 9; i++) {
        if (self->field_14C->field_29CC[i].field_00 == 2
            && (self->field_14C->field_29CC[i].field_04 & 0x10000) != 0
            && self->field_14C->field_29CC[i].field_0C == 3
            && self->field_14C->field_29CC[i].field_0E == id) {
            result = 1;
            break;
        }
    }
    return result;
}
