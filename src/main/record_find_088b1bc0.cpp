/* Binary-searches the global record table at 0x089D728C for an id; 0 for id -1. */
struct Record088B1BC0 {
    int id;
    int field_04;
    int field_08;
    int field_0C;
};

struct Info088B1BC0 {
    char pad_00[8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088B1BC0 {
    signed char descending;

    Order088B1BC0() : descending(0) {}
};

extern "C" {

extern Info088B1BC0 *D_089D7288;
extern Record088B1BC0 *D_089D728C;

Record088B1BC0 *func_088B1A78(Record088B1BC0 *begin, Record088B1BC0 *end, const Record088B1BC0 *key, Order088B1BC0 order);

Record088B1BC0 *func_088B1BC0(int id)
{
    Record088B1BC0 *result = 0;
    if (id != -1) {
        Record088B1BC0 key;
        key.id = id;
        result = func_088B1A78(D_089D728C, D_089D728C + D_089D7288->count, &key, Order088B1BC0());
    }
    return result;
}

}
