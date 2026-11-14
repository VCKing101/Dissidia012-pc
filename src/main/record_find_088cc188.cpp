/* Binary-searches the record table at 0x089D77FC (0x10-byte records) for an id. */
struct Record088CC188 {
    short id;
    short fields[7];
};

struct Header088CC188 {
    char pad_00[0x8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088CC188 {
    signed char descending;

    Order088CC188() : descending(0) {}
};

extern "C" {

extern Header088CC188 *D_089D77F8;
extern Record088CC188 *D_089D77FC;

Record088CC188 *func_088CC034(Record088CC188 *begin, Record088CC188 *end, const Record088CC188 *key, Order088CC188 order);

void func_088CC188(int id)
{
    Record088CC188 key;
    key.id = id;
    func_088CC034(D_089D77FC, D_089D77FC + D_089D77F8->count, &key, Order088CC188());
}

}
