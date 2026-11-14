/* Binary-searches the global record table at 0x089D73EC for an id; 0 for id -1. */
struct Record088BFF40 {
    short id;
    short field_02;
    int field_04;
};

struct Info088BFF40 {
    char pad_00[8];
    short count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088BFF40 {
    signed char descending;

    Order088BFF40() : descending(0) {}
};

extern "C" {

extern Info088BFF40 *D_089D73E4;
extern Record088BFF40 *D_089D73EC;

Record088BFF40 *func_088BFC94(Record088BFF40 *begin, Record088BFF40 *end, const Record088BFF40 *key, Order088BFF40 order);

Record088BFF40 *func_088BFF40(int id)
{
    Record088BFF40 *result = 0;
    if (id != -1) {
        Record088BFF40 key;
        key.id = id;
        result = func_088BFC94(D_089D73EC, D_089D73EC + D_089D73E4->count, &key, Order088BFF40());
    }
    return result;
}

}
