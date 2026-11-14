/* Whether no active member with flag 0x40000 has a part still missing flag 0x20. */
struct MemberPart {
    char pad_00[0x20];
    unsigned short field_20;

    static bool flagged(MemberPart *part) { return (part->field_20 & 0x20) != 0; }
};

struct MemberSlot {
    int field_00;
    unsigned int field_04;
    char pad_08[0x84 - 0x8];
};

struct MemberTable {
    char pad_0000[0x6B0];
    MemberPart *field_6B0[9];
    char pad_06D4[0x2731 - 0x6D4];
    signed char field_2731;
    char pad_2732[0x29CC - 0x2732];
    MemberSlot field_29CC[9];
};

struct MemberOwner {
    char pad_000[0x14C];
    MemberTable *field_14C;
};

extern "C" {
int func_089F66E0(MemberOwner *self)
{
    int i = 0;
    int result = 1;
    MemberTable *table = self->field_14C;
    for (; i < table->field_2731; i++) {
        if ((self->field_14C->field_29CC[i].field_04 & 0x40000) != 0 && !MemberPart::flagged(self->field_14C->field_6B0[i])) {
            result = 0;
            break;
        }
    }
    return result;
}
}
