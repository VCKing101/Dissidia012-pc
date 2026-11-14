/* Binary-searches the record table at 0x089D771C (0x12-byte records) for an id. */
struct Record088C5DF0 {
    short id;
    short fields[8];
};

struct Header088C5DF0 {
    char pad_00[0x8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088C5DF0 {
    signed char descending;

    Order088C5DF0() : descending(0) {}
};

extern "C" {

extern Header088C5DF0 *D_089D7718;
extern Record088C5DF0 *D_089D771C;

Record088C5DF0 *func_088C5CE8(Record088C5DF0 *begin, Record088C5DF0 *end, const Record088C5DF0 *key, Order088C5DF0 order);

Record088C5DF0 *func_088C5DF0(int id)
{
    Record088C5DF0 *result = 0;
    if (id >= 0) {
        Record088C5DF0 key;
        key.id = id;
        result = func_088C5CE8(D_089D771C, D_089D771C + D_089D7718->count, &key, Order088C5DF0());
    }
    return result;
}

}
