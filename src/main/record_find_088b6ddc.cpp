/* Binary-searches a table of 8-byte records for an id byte. */
struct Record088B6DDC {
    signed char id;
    char field_01;
    short field_02;
    short field_04;
    short field_06;
};

struct Table088B6DDC {
    int field_00;
    int count;
    Record088B6DDC *records;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088B6DDC {
    signed char descending;

    Order088B6DDC() : descending(0) {}
};

extern "C" {

Record088B6DDC *func_088B6D34(Record088B6DDC *begin, Record088B6DDC *end, const Record088B6DDC *key, Order088B6DDC order);

void func_088B6DDC(Table088B6DDC *table, int id)
{
    Record088B6DDC key;
    key.id = id;
    func_088B6D34(table->records, table->records + table->count, &key, Order088B6DDC());
}

}
