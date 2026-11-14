/* Fills a slot from its kind's table entry and sets the matching flag bits. */
typedef struct SlotKind {
    signed char field_00;
    signed char field_01;
    signed char field_02;
    char pad_03;
    short field_04;
} SlotKind;

typedef struct SlotData {
    char field_00;
    signed char field_01;
    char pad_02[0x2];
    unsigned int field_04;
    char pad_08[0x4];
    short field_0C;
    signed char field_0E;
    signed char field_0F;
    short field_10;
    signed char field_12;
    signed char field_13;
} SlotData;

extern short *func_089F0AE4(void *self, SlotData *slot);
extern SlotKind *func_089F1CE0(void *self, int kind);
extern void func_089F1E50(void *self, SlotData *slot, int mode);

void func_089F1CFC(void *self, SlotData *slot)
{
    SlotKind *kind;
    signed char first;
    slot->field_01 = *func_089F0AE4(self, slot);
    kind = func_089F1CE0(self, slot->field_01);
    slot->field_0C = slot->field_01;
    slot->field_0E = kind->field_01;
    slot->field_0F = kind->field_02;
    slot->field_10 = kind->field_04;
    first = kind->field_00;
    slot->field_13 = 0;
    slot->field_12 = first;
    if (kind->field_01 != 0) {
        slot->field_04 = slot->field_04 | 0x4000;
    }
    func_089F1E50(self, slot, 0);
    if ((slot->field_04 & 0x8000) != 0) {
        slot->field_04 = slot->field_04 | 0x400000;
    }
}
