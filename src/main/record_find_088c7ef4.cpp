/* Binary-searches the global record table at 0x089D7748 for an id. */
struct Record088C7EF4 {
    short id;
    short field_02;
    int field_04;
};

struct Info088C7EF4 {
    char pad_00[0xC];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088C7EF4 {
    signed char descending;

    Order088C7EF4() : descending(0) {}
};

extern "C" {

extern Info088C7EF4 *D_089D7740;
extern Record088C7EF4 *D_089D7748;

Record088C7EF4 *func_088C7D78(Record088C7EF4 *begin, Record088C7EF4 *end, const Record088C7EF4 *key, Order088C7EF4 order);

void func_088C7EF4(int id)
{
    Record088C7EF4 key;
    key.id = id;
    func_088C7D78(D_089D7748, D_089D7748 + D_089D7740->count, &key, Order088C7EF4());
}

}
