typedef struct CardRec { short field_00; signed char kind; char field_03; short field_04; short id; char rest[0x4E - 8]; } CardRec;
typedef struct CardTable { char pad_00[0x10]; short count; short pad_12; CardRec recs[1]; } CardTable;
typedef struct CardOwner { char pad_00[0xC]; CardTable *table; } CardOwner;
CardRec *func_08925C90(CardOwner *owner, signed char kind, short id)
{
    int i;
    CardRec *result = 0;
    for (i = 0; i < owner->table->count; i++) {
        if (owner->table->recs[i].kind == kind && owner->table->recs[i].id == id) {
            result = &owner->table->recs[i];
            break;
        }
    }
    return result;
}
