/* Flags the state after picking a member; an empty member row adds another flag. */
typedef struct MemberRow {
    char pad_000[0x6];
    short field_06;
    char pad_008[0x118 - 0x8];
} MemberRow;

typedef struct MemberState {
    char pad_0000[0x30];
    MemberRow *field_30;
    char pad_0034[0x2724 - 0x34];
    unsigned int field_2724;
} MemberState;

typedef struct MemberOwner {
    char pad_000[0x14C];
    MemberState *field_14C;
} MemberOwner;

typedef struct MemberRef {
    char pad_00[0x4];
    short field_04;
} MemberRef;

extern MemberRef *func_089FCA28(MemberOwner *self, unsigned short id);

void func_089F2578(MemberOwner *self, short id)
{
    MemberRef *ref = func_089FCA28(self, id);
    MemberState *state;
    short row;
    self->field_14C->field_2724 = self->field_14C->field_2724 | 0x10000;
    row = ref->field_04;
    state = self->field_14C;
    if (state->field_30[row].field_06 == 0) {
        state->field_2724 = state->field_2724 | 0x1000000;
    }
}
