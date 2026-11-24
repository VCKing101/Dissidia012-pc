/* Copies row `id` of one of the two row tables into `out`; returns 0, -1
   for bad arguments or -2 when the row does not exist. */
typedef struct TableRow {
    int words[0x40 / 4];
} TableRow;

typedef struct Table Table;

extern TableRow *func_088C6D0C(int id);

int func_08891CA4(Table *self, TableRow *out, unsigned int which, int id)
{
    TableRow *row;

    if (out == 0 || which >= 2 || id < 0) {
        return -1;
    }
    row = func_088C6D0C(id);
    if (row == 0) {
        return -2;
    }
    *out = *row;
    return 0;
}
