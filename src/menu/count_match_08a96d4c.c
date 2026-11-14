typedef struct MatchRec {
    char pad_00[2];
    short field_02;
    char pad_04[4];
} MatchRec;

typedef struct MatchInner {
    char pad_00[4];
    int field_04;
    char pad_08[0x70 - 8];
    MatchRec *field_70;
} MatchInner;

typedef struct MatchOwner {
    char pad_00[4];
    MatchInner *field_04;
} MatchOwner;

int func_08A96D4C(MatchOwner *self, int key)
{
    int i = 0;
    int count = self->field_04->field_04;
    int hits = 0;
    if (i < count) {
        MatchRec *rec = self->field_04->field_70;
        do {
            if (rec->field_02 == key) {
                hits++;
            }
            i++;
            rec++;
        } while (i < count);
    }
    return hits;
}
