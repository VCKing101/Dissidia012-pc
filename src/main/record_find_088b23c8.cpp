/* Binary-searches the global record table at 0x089D72B0 for an id. */
struct Record088B23C8 {
    short id;
    short field_02;
    int field_04;
};

struct Info088B23C8 {
    char pad_00[8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088B23C8 {
    signed char descending;

    Order088B23C8() : descending(0) {}
};

extern "C" {

extern Info088B23C8 *D_089D72A8;
extern Record088B23C8 *D_089D72B0;

Record088B23C8 *func_088B21BC(Record088B23C8 *begin, Record088B23C8 *end, const Record088B23C8 *key, Order088B23C8 order);

void func_088B23C8(int id)
{
    Record088B23C8 key;
    key.id = id;
    func_088B21BC(D_089D72B0, D_089D72B0 + D_089D72A8->count, &key, Order088B23C8());
}

}
