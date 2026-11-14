/* Updates every kind-1 slot in the active range. */
typedef struct UpdSlot {
    unsigned char field_00;
    char pad_01[0x84 - 0x1];
} UpdSlot;

typedef struct UpdState {
    char pad_0000[0x2731];
    signed char field_2731;
    char pad_2732[0x29CC - 0x2732];
    UpdSlot field_29CC[9];
} UpdState;

typedef struct UpdOwner {
    char pad_000[0x14C];
    UpdState *field_14C;
} UpdOwner;

extern void func_089F1580(UpdOwner *self, UpdSlot *slot);

void func_089F2088(UpdOwner *self)
{
    int i;
    for (i = 0; i < self->field_14C->field_2731; i++) {
        if (self->field_14C->field_29CC[i].field_00 == 1) {
            func_089F1580(self, &self->field_14C->field_29CC[i]);
        }
    }
}
