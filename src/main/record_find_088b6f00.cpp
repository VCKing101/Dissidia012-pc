/* Binary-searches the global record table at 0x089D7364 for an id. */
struct Record088B6F00 {
    int id;
    int field_04;
    int field_08;
    int field_0C;
};

struct Info088B6F00 {
    char pad_00[8];
    int count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088B6F00 {
    signed char descending;

    Order088B6F00() : descending(0) {}
};

extern "C" {

extern Info088B6F00 *D_089D7360;
extern Record088B6F00 *D_089D7364;

Record088B6F00 *func_088B6C8C(Record088B6F00 *begin, Record088B6F00 *end, const Record088B6F00 *key, Order088B6F00 order);

void func_088B6F00(int id)
{
    Record088B6F00 key;
    key.id = id;
    func_088B6C8C(D_089D7364, D_089D7364 + D_089D7360->count, &key, Order088B6F00());
}

}
