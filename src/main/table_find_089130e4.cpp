extern "C" {
typedef struct TableRec {
    int id;
    int field_04[8];
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

TableRec *func_089130E4(TableRef *ref, int id)
{
    return RecTable::find(ref->table, id);
}
}
