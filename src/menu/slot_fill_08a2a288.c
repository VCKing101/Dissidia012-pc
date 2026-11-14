typedef struct SlotRec {
    short field_00;
    short field_02;
    short field_04;
    unsigned short field_06;
    char pad_08[0x18];
} SlotRec;

typedef struct SlotBlock {
    char pad_000[0xC38];
    SlotRec field_C38[1];
} SlotBlock;

typedef struct SlotOwner {
    char pad_000[0x138];
    SlotBlock *field_138;
} SlotOwner;

extern unsigned short D_08A2E722[];

int func_08A2A288(SlotOwner *self, int index)
{
    SlotRec *rec = &self->field_138->field_C38[index];
    rec->field_00 = (short)index + 0x31;
    rec->field_02 = 0x16;
    rec->field_04 = 0x38;
    rec->field_06 = D_08A2E722[index];
    return 1;
}
