/* Finds the hint mark at (x, y) whose flags include the mask; -1 if none. */
typedef struct HintMark {
    unsigned char field_00;
    char pad_01[0x3];
    short field_04;
    short field_06;
} HintMark;

typedef struct HintMarks {
    char pad_00000[0x26D6C];
    HintMark field_26D6C[(0x2D16C - 0x26D6C) / 8];
    int field_2D16C;
} HintMarks;

typedef struct HintState {
    char pad_0000[0x38A0];
    HintMarks field_38A0;
} HintState;

typedef struct HintOwner {
    char pad_000[0x14C];
    HintState *field_14C;
} HintOwner;

int func_089FDE6C(HintOwner *self, short x, short y, unsigned char mask)
{
    HintMarks *marks = &self->field_14C->field_38A0;
    int found = -1;
    int i;
    for (i = 0; i < marks->field_2D16C; i++) {
        if ((marks->field_26D6C[i].field_00 & mask) == mask && x == marks->field_26D6C[i].field_04 &&
            y == marks->field_26D6C[i].field_06) {
            found = i;
            break;
        }
    }
    return found;
}
