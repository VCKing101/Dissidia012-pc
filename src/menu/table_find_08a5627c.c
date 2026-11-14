typedef struct TableRow {
    short field_00;
    short field_02;
    unsigned short field_04;
} TableRow;

extern TableRow D_08A58478;

unsigned short func_08A5627C(int key)
{
    TableRow *row = &D_08A58478;
    short id = row->field_00;
    while (1) {
        if (id == key) {
            return row->field_04;
        }
        if (id == -2) {
            break;
        }
        row = row + 1;
        id = row->field_00;
    }
    return 0x64B1;
}
