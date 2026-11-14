typedef struct Ent16 { int a; int b; int c; int d; } Ent16;
typedef struct Ent16Table { int field_00; int field_04; int count; int field_0C; Ent16 entries[1]; } Ent16Table;
typedef struct Ent16View { Ent16Table *table; } Ent16View;
Ent16 *func_08869A7C(Ent16View *view, int idx)
{
    Ent16Table *t;
    Ent16 *arr;
    if (idx < 0 || idx >= view->table->count) {
        return 0;
    }
    t = view->table;
    arr = t->entries;
    return &arr[idx];
}
