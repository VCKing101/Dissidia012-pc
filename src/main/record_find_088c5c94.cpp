/* Binary-searches the record table at 0x089D7710 (0x14-byte records) for an id. */
struct Record088C5C94 {
    short id;
    short fields[9];
};

struct Header088C5C94 {
    char pad_00[0x12];
    short count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088C5C94 {
    signed char descending;

    Order088C5C94() : descending(0) {}
};

extern "C" {

extern Header088C5C94 *D_089D76F8;
extern Record088C5C94 *D_089D7710;

Record088C5C94 *func_088C58DC(Record088C5C94 *begin, Record088C5C94 *end, const Record088C5C94 *key, Order088C5C94 order);

Record088C5C94 *func_088C5C94(int id)
{
    Record088C5C94 *result = 0;
    if (id >= 0) {
        Record088C5C94 key;
        key.id = id;
        result = func_088C58DC(D_089D7710, D_089D7710 + D_089D76F8->count, &key, Order088C5C94());
    }
    return result;
}

}
