/* Binary-searches the global record table at 0x089D76FC for an id; 0 for a negative id. */
struct Record088C5B34 {
    short id;
    short fields[99];
};

struct Info088C5B34 {
    char pad_00[8];
    short count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088C5B34 {
    signed char descending;

    Order088C5B34() : descending(0) {}
};

extern "C" {

extern Info088C5B34 *D_089D76F8;
extern Record088C5B34 *D_089D76FC;

Record088C5B34 *func_088C561C(Record088C5B34 *begin, Record088C5B34 *end, const Record088C5B34 *key, Order088C5B34 order);

Record088C5B34 *func_088C5B34(int id)
{
    Record088C5B34 *result = 0;
    if (id >= 0) {
        Record088C5B34 key;
        key.id = id;
        result = func_088C561C(D_089D76FC, D_089D76FC + D_089D76F8->count, &key, Order088C5B34());
    }
    return result;
}

}
