/* Binary-searches the global record table at 0x089D77D4 for an id; 0 for id -1. */
struct Record088CAF10 {
    short id;
    short fields[3];
};

struct Info088CAF10 {
    char pad_00[8];
    short count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088CAF10 {
    signed char descending;

    Order088CAF10() : descending(0) {}
};

extern "C" {

extern Info088CAF10 *D_089D77D0;
extern Record088CAF10 *D_089D77D4;

Record088CAF10 *func_088CAD10(Record088CAF10 *begin, Record088CAF10 *end, const Record088CAF10 *key, Order088CAF10 order);

Record088CAF10 *func_088CAF10(int id)
{
    Record088CAF10 *result = 0;
    if (id != -1) {
        Record088CAF10 key;
        key.id = id;
        result = func_088CAD10(D_089D77D4, D_089D77D4 + D_089D77D0->count, &key, Order088CAF10());
    }
    return result;
}

}
