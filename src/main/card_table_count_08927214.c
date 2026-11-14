typedef struct CardRec { short field_00; signed char kind; char field_03; short field_04; short id; short slot; char rest[0x4E - 10]; } CardRec;
typedef struct CardTable { char pad_00[0x10]; short count; short pad_12; CardRec recs[1]; } CardTable;
typedef struct CardOwner { char pad_00[0xC]; CardTable *table; } CardOwner;
int func_08927214(CardOwner *owner, int kind)
{
    int i;
    int n = 0;
    for (i = 0; i < owner->table->count; i++) {
        if (owner->table->recs[i].kind == kind && owner->table->recs[i].slot != -1) {
            n++;
        }
    }
    return n;
}
