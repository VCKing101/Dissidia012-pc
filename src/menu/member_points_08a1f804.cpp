/* Adds to a unit's two point totals (each capped at 9999) and writes the member's stored totals. */
struct PointUnit {
    char pad_00[0xE8];
    short field_E8;
    short field_EA;
    short field_EC;
    short field_EE;
};

/* Members are 0x24C bytes; the team's member index table sits at +0x677C. */
struct PointMember {
    char pad_000[0x24C];
};

struct PointMemberTotals {
    char pad_0000[0x2DFC];
    unsigned short field_2DFC;
    unsigned short field_2DFE;
};

struct PointTeam {
    char pad_0000[0x677C];
    short field_677C[1];
};

struct PointOwner {
    char pad_000[0x154];
    PointMember *field_154;

    static PointMemberTotals *member(PointOwner *self, int index)
    {
        return (PointMemberTotals *)&self->field_154[((PointTeam *)self->field_154)->field_677C[index]];
    }
    static int cap(int value)
    {
        int capped = 9999;
        if (value < 9999) {
            capped = value;
        }
        return capped;
    }
};

extern "C" {
void func_08A1F804(PointOwner *self, int index, PointUnit *unit, int add_a, int add_b)
{
    unit->field_EC = PointOwner::cap(unit->field_EC + add_a);
    unit->field_EE = PointOwner::cap(unit->field_EE + add_b);
    PointOwner::member(self, index)->field_2DFE = unit->field_E8 + unit->field_EC;
    if (PointOwner::member(self, index)->field_2DFE >= 10000) {
        PointOwner::member(self, index)->field_2DFE = 9999;
    }
    unsigned short total = unit->field_EA + unit->field_EE;
    if (total >= 10000) {
        total = 9999;
    }
    PointOwner::member(self, index)->field_2DFC = PointOwner::member(self, index)->field_2DFE + total;
}
}
