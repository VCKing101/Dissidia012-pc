/* Binary-searches the record table at 0x089D7700 (0x2c-byte records) for an id. */
struct Record088C5B90 {
    short id;
    short fields[21];
};

struct Header088C5B90 {
    char pad_00[0xA];
    short count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088C5B90 {
    signed char descending;

    Order088C5B90() : descending(0) {}
};

extern "C" {

extern Header088C5B90 *D_089D76F8;
extern Record088C5B90 *D_089D7700;

Record088C5B90 *func_088C56D0(Record088C5B90 *begin, Record088C5B90 *end, const Record088C5B90 *key, Order088C5B90 order);

Record088C5B90 *func_088C5B90(int id)
{
    Record088C5B90 *result = 0;
    if (id >= 0) {
        Record088C5B90 key;
        key.id = id;
        result = func_088C56D0(D_089D7700, D_089D7700 + D_089D76F8->count, &key, Order088C5B90());
    }
    return result;
}

}
