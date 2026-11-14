/* Binary-searches the global record table at 0x089D7384 for an id; 0 for id -1. */
struct Record088BB708 {
    short id;
    short fields[9];
};

struct Info088BB708 {
    char pad_00[8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088BB708 {
    signed char descending;

    Order088BB708() : descending(0) {}
};

extern "C" {

extern Info088BB708 *D_089D7380;
extern Record088BB708 *D_089D7384;

Record088BB708 *func_088BB600(Record088BB708 *begin, Record088BB708 *end, const Record088BB708 *key, Order088BB708 order);

Record088BB708 *func_088BB708(int id)
{
    Record088BB708 *result = 0;
    if (id != -1) {
        Record088BB708 key;
        key.id = id;
        result = func_088BB600(D_089D7384, D_089D7384 + D_089D7380->count, &key, Order088BB708());
    }
    return result;
}

}
