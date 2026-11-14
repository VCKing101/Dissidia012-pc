/* Binary-searches the record table at 0x089D7704 (0xc-byte records) for an id. */
struct Record088C5BE8 {
    short id;
    short fields[5];
};

struct Header088C5BE8 {
    char pad_00[0xC];
    short count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088C5BE8 {
    signed char descending;

    Order088C5BE8() : descending(0) {}
};

extern "C" {

extern Header088C5BE8 *D_089D76F8;
extern Record088C5BE8 *D_089D7704;

Record088C5BE8 *func_088C5780(Record088C5BE8 *begin, Record088C5BE8 *end, const Record088C5BE8 *key, Order088C5BE8 order);

Record088C5BE8 *func_088C5BE8(int id)
{
    Record088C5BE8 *result = 0;
    if (id >= 0) {
        Record088C5BE8 key;
        key.id = id;
        result = func_088C5780(D_089D7704, D_089D7704 + D_089D76F8->count, &key, Order088C5BE8());
    }
    return result;
}

}
