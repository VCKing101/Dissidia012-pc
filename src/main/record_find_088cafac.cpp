/* Binary-searches a table of 0x24-byte records for an id; 0 for id -1. */
struct Record088CAFAC {
    int id;
    int fields[8];
};
struct Info088CAFAC {
    char pad_00[8];
    int count;
};
struct Table088CAFAC {
    Info088CAFAC *info;
    Record088CAFAC *records;
};
struct Order088CAFAC {
    signed char descending;

    Order088CAFAC() : descending(0) {}
};
extern "C" {
Record088CAFAC *func_088CADB8(Record088CAFAC *begin, Record088CAFAC *end, const Record088CAFAC *key, Order088CAFAC order);
Record088CAFAC *func_088CAFAC(Table088CAFAC *t, int id)
{
    Record088CAFAC *result = 0;
    if (id != -1) {
        Record088CAFAC key;
        key.id = id;
        result = func_088CADB8(t->records, t->records + t->info->count, &key, Order088CAFAC());
    }
    return result;
}
}
