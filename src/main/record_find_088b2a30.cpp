/* Binary-searches the global record table at 0x089D72C0 for an id; 0 for id -1. */
struct Record088B2A30 {
    short id;
    short fields[3];
};

struct Info088B2A30 {
    char pad_00[0xC];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088B2A30 {
    signed char descending;

    Order088B2A30() : descending(0) {}
};

extern "C" {

extern Info088B2A30 *D_089D72B8;
extern Record088B2A30 *D_089D72C0;

Record088B2A30 *func_088B27F4(Record088B2A30 *begin, Record088B2A30 *end, const Record088B2A30 *key, Order088B2A30 order);

Record088B2A30 *func_088B2A30(int id)
{
    Record088B2A30 *result = 0;
    if (id != -1) {
        Record088B2A30 key;
        key.id = id;
        result = func_088B27F4(D_089D72C0, D_089D72C0 + D_089D72B8->count, &key, Order088B2A30());
    }
    return result;
}

}
