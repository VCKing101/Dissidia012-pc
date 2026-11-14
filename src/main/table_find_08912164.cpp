extern "C" {
typedef struct TableRec {
    short id;
    short field_02;
    int field_04;
    int field_08;
    int field_0C;
    int field_10[7];
} TableRec;

typedef struct RecTable {
    int field_00;
    int count;
    TableRec recs[1];

    static TableRec *find(RecTable *t, int id)
    {
        for (int i = 0; i < t->count; i++) {
            if (((TableRec *)((char *)t->recs + i * sizeof(TableRec)))->id == id) {
                return (TableRec *)((char *)t->recs + i * sizeof(TableRec));
            }
        }
        return 0;
    }
} RecTable;

typedef struct TableRef {
    RecTable *table;
} TableRef;

TableRec *func_08912164(TableRef *ref, int id)
{
    return RecTable::find(ref->table, id);
}
}
