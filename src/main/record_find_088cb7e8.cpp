/* Binary-searches the record table at 0x089D77EC (0x30-byte records) for an id. */
struct Record088CB7E8 {
    short id;
    short field_02;
    int fields[11];
};

struct Header088CB7E8 {
    char pad_00[0x8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088CB7E8 {
    signed char descending;

    Order088CB7E8() : descending(0) {}
};

extern "C" {

extern Header088CB7E8 *D_089D77E8;
extern Record088CB7E8 *D_089D77EC;

Record088CB7E8 *func_088CB6DC(Record088CB7E8 *begin, Record088CB7E8 *end, const Record088CB7E8 *key, Order088CB7E8 order);

void func_088CB7E8(int id)
{
    Record088CB7E8 key;
    key.id = id;
    func_088CB6DC(D_089D77EC, D_089D77EC + D_089D77E8->count, &key, Order088CB7E8());
}

}
