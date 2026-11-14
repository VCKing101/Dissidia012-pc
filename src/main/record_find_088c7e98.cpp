/* Binary-searches the record table at 0x089D7744 (0x24-byte records) for an id. */
struct Record088C7E98 {
    short id;
    short fields[17];
};

struct Header088C7E98 {
    char pad_00[0x8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088C7E98 {
    signed char descending;

    Order088C7E98() : descending(0) {}
};

extern "C" {

extern Header088C7E98 *D_089D7740;
extern Record088C7E98 *D_089D7744;

Record088C7E98 *func_088C7CCC(Record088C7E98 *begin, Record088C7E98 *end, const Record088C7E98 *key, Order088C7E98 order);

Record088C7E98 *func_088C7E98(int id)
{
    Record088C7E98 key;
    Record088C7E98 *result = 0;
    if (id != -1) {
        key.id = id;
        result = func_088C7CCC(D_089D7744, D_089D7744 + D_089D7740->count, &key, Order088C7E98());
    }
    return result;
}

}
