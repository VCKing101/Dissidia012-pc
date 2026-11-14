/* Clears the party header and resets the five 0x2D8-byte members. */
struct PartyMember {
    char data[0x2D8];
};

struct Party {
    PartyMember members[5];
    char pad_E38[0];
    int field_E38;
    short field_E3C;
    char pad_E3E[0xE56 - 0xE3E];
    short field_E56;
};

extern "C" {
extern void func_088F4F14(PartyMember *member);

void func_088F4F3C(Party *party)
{
    party->field_E38 = 0;
    party->field_E3C = 0;
    party->field_E56 = 0;
    for (int i = 0; i < 5; i++) {
        func_088F4F14(&party->members[i]);
    }
}
}
