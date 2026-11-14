typedef struct CardRec { short field_00; signed char kind; char field_03; short field_04; short id; char rest[0x4E - 8]; } CardRec;
typedef struct CardTable { char pad_00[0x10]; short count; short pad_12; CardRec recs[1]; } CardTable;
typedef struct CardOwner { char pad_00[0xC]; CardTable *table; } CardOwner;
int func_08925D34(CardOwner *owner, short id)
{
    int i;
    int result = -1;
    for (i = 0; i < owner->table->count; i++) {
        if (owner->table->recs[i].field_00 == id) {
            result = i;
            break;
        }
    }
    return result;
}
