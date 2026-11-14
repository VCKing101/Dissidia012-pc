/* Binary-searches the global record table at 0x089D7310 for an id. */
struct Record088B6640 {
    short id;
    short field_02;
    int field_04;
};

struct Info088B6640 {
    char pad_00[8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088B6640 {
    signed char descending;

    Order088B6640() : descending(0) {}
};

extern "C" {

extern Info088B6640 *D_089D730C;
extern Record088B6640 *D_089D7310;

Record088B6640 *func_088B6598(Record088B6640 *begin, Record088B6640 *end, const Record088B6640 *key, Order088B6640 order);

void func_088B6640(int id)
{
    Record088B6640 key;
    key.id = id;
    func_088B6598(D_089D7310, D_089D7310 + D_089D730C->count, &key, Order088B6640());
}

}
