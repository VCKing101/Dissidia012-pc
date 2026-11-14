/* Binary-searches the record table at 0x089D7708 (0x184-byte records) for an id. */
struct Record088C5C3C {
    short id;
    short fields[193];
};

struct Header088C5C3C {
    char pad_00[0xE];
    short count;
};

/* Comparison order passed by value; 0 = ascending. */
struct Order088C5C3C {
    signed char descending;

    Order088C5C3C() : descending(0) {}
};

extern "C" {

extern Header088C5C3C *D_089D76F8;
extern Record088C5C3C *D_089D7708;

Record088C5C3C *func_088C582C(Record088C5C3C *begin, Record088C5C3C *end, const Record088C5C3C *key, Order088C5C3C order);

Record088C5C3C *func_088C5C3C(int id)
{
    Record088C5C3C *result = 0;
    if (id >= 0) {
        Record088C5C3C key;
        key.id = id;
        result = func_088C582C(D_089D7708, D_089D7708 + D_089D76F8->count, &key, Order088C5C3C());
    }
    return result;
}

}
