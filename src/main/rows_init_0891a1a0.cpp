extern "C" {
typedef struct IdRow {
    short ids[6];
    short current;
    short count;
} IdRow;

typedef struct IdRows {
    IdRow rows[2];
    int limit;                  /* 0x20 */
} IdRows;

void func_0891A1A0(IdRows *rows)
{
    for (int i = 0; i < 2; i++) {
        IdRow *row = &rows->rows[i];
        for (int j = 0; j < 6; j++) {
            row->ids[j] = -1;
        }
        row->current = -1;
        row->count = 0;
    }
    rows->limit = 100;
}
}
