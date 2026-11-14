/* Binary-searches the global record table at 0x089D73BC for an id. */
struct Record088BD278 {
    short id;
    short fields[13];
};

struct Info088BD278 {
    char pad_00[8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088BD278 {
    signed char descending;

    Order088BD278() : descending(0) {}
};

extern "C" {

extern Info088BD278 *D_089D73B8;
extern Record088BD278 *D_089D73BC;

Record088BD278 *func_088BD10C(Record088BD278 *begin, Record088BD278 *end, const Record088BD278 *key, Order088BD278 order);

void func_088BD278(int id)
{
    Record088BD278 key;
    key.id = id;
    func_088BD10C(D_089D73BC, D_089D73BC + D_089D73B8->count, &key, Order088BD278());
}

}
