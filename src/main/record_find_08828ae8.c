typedef struct Record08828AE8 {
    short id;
    unsigned char unknown_02[2];
    int field_04;
    unsigned char unknown_08[0xC4];
} Record08828AE8;

typedef struct Info08828AE8 {
    unsigned char unknown_00[8];
    int count;
} Info08828AE8;

typedef struct Table08828AE8 {
    Info08828AE8 *info;
    Record08828AE8 *records;
} Table08828AE8;

typedef struct Order08828AE8 {
    signed char descending;
} Order08828AE8;

int func_088289D0(Record08828AE8 *begin, Record08828AE8 *end, const Record08828AE8 *key, Order08828AE8 order);

int func_08828AE8(Table08828AE8 *table, int id) {
    Order08828AE8 order;
    Record08828AE8 key;
    int result = 0;
    if (id != -1) {
        Record08828AE8 *records;
        Record08828AE8 *end;
        key.id = id;
        records = table->records;
        end = &records[table->info->count];
        order.descending = 0;
        result = func_088289D0(records, end, &key, order);
    }
    return result;
}
