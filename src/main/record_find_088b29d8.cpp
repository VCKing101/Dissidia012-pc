/* Binary-searches the global record table at 0x089D72BC for an id; 0 for id -1. */
struct Record088B29D8 {
    short id;
    short field_02;
    int fields[16];
};

struct Info088B29D8 {
    char pad_00[8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088B29D8 {
    signed char descending;

    Order088B29D8() : descending(0) {}
};

extern "C" {

extern Info088B29D8 *D_089D72B8;
extern Record088B29D8 *D_089D72BC;

Record088B29D8 *func_088B2748(Record088B29D8 *begin, Record088B29D8 *end, const Record088B29D8 *key, Order088B29D8 order);

Record088B29D8 *func_088B29D8(int id)
{
    Record088B29D8 *result = 0;
    if (id != -1) {
        Record088B29D8 key;
        key.id = id;
        result = func_088B2748(D_089D72BC, D_089D72BC + D_089D72B8->count, &key, Order088B29D8());
    }
    return result;
}

}
